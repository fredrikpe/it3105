
/* Image processing with simd instructions.
 *
 * Calculates the difference of gassians (DoG, an algorithm
 * used for image feature enhancement) beetween two images convoluted 5 
 * times with a gaussian blur in different sizes.
 */
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ppm.h"


// Image from:
// http://7-themes.com/6971875-funny-flowers-pictures.html

/* Uses a vector of four element (4 floats, 16 bytes), the last being a dummy. Using 
 * floating point instead of double gives a lower runtime and some pixel errors.
 */
typedef float v4f __attribute__ ((vector_size (16)));
typedef struct {
    v4f rgb;
} AccuratePixel;

typedef struct {
    int x, y;
    AccuratePixel *data; // TODO: this might require some changes for a SIMD version too.
} AccurateImage;

AccuratePixel *line_buffer;

// Convert ppm to high precision format.
AccurateImage *convertImageToNewFormat(PPMImage *image)
{
    // Make a copy
    AccurateImage *imageAccurate;
    imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));

    imageAccurate->data = (AccuratePixel*)malloc(sizeof(AccuratePixel)*image->x*image->y);

    for(int i = 0; i < image->x * image->y; i++) {
        v4f rgb = {(float) image->data[i].red, 
                   (float) image->data[i].green, 
                   (float) image->data[i].blue, 
                   0};
        imageAccurate->data[i].rgb = rgb;
    }
    imageAccurate->x = image->x;
    imageAccurate->y = image->y;

    return imageAccurate;
}

AccurateImage *createEmptyImage(PPMImage *image)
{
    AccurateImage *imageAccurate;
    imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));
    imageAccurate->data = (AccuratePixel*)malloc(sizeof(AccuratePixel)*image->x*image->y);
    imageAccurate->x = image->x;
    imageAccurate->y = image->y;

    return imageAccurate;
}

// free memory of an AccurateImage
void freeImage(AccurateImage *image)
{
    free(image->data);
    free(image);
}

/* Passes through one line of the image setting imageOut to the average of size + size pixels.
 */
void horizontalAvg(AccurateImage *imageOut, AccuratePixel *line_buffer, float reciprocal, int W, int start_i, int end_i, int i, int size)
{
    v4f sum_rgb = {0, 0, 0, 0};
    int j;
    int start_j = 0;
    int end_j = size - 1;

    // Initiate with values
    for (j=0; j < size; j++){
        sum_rgb += line_buffer[j].rgb;
    }
    // Start edge
    for(j=0; j <= size; j++)
    {
        end_j++;
        sum_rgb +=line_buffer[end_j].rgb;
        reciprocal = 1.0 / ((end_j+1)*(end_i-start_i+1));
        imageOut->data[W*i + j].rgb = sum_rgb * reciprocal;
    }
    // Middle part
    for(; j < W-size; j++)
    {
        start_j++;
        end_j++;
        sum_rgb += line_buffer[end_j].rgb-line_buffer[start_j-1].rgb;
        imageOut->data[W*i + j].rgb = sum_rgb * reciprocal;
    }
    // End edge
    for(; j < W; j++)
    {
        start_j++;
        sum_rgb -= line_buffer[start_j-1].rgb;
        reciprocal = 1.0 / ((end_j-start_j+1)*(end_i-start_i+1));
        imageOut->data[W*i + j].rgb = sum_rgb * reciprocal;
    }
}

/* Image convolution with gaussian blur (square box average), using the fact that average of box 
 * surrounding pixel equals the sum of the vertical plus horizontal averages. Stores intermediate 
 * values (the vertical averages) in a line buffer.
 *       The outer for-loop has been seperated in three to minimize branching, believed to have 
 * performance benefits, although this is not 100 % certain. Minimizing divisions is very important.
 */
void gaussianBlur(AccurateImage *imageOut, AccurateImage *imageIn, int size)
{
    float reciprocal = 1.0 / (size * 2 + 1);

    int W = imageIn->x;
    int H = imageIn->y;

    // Reset the line_buffer
    memset(line_buffer, 0, W * sizeof(AccuratePixel));

    int start_i = 0;
    int end_i = size - 1;

    int i;

    // Initiate line_buffer with values
    for(i = 0; i < size; i++){
        for(int j=0; j<W; j++){
            line_buffer[j].rgb += imageIn->data[W*i+j].rgb;
        }
    }
    // Start edge
    for(i = 0; i <= size; i++) {
        end_i++;
        // first and last line considered  by the computation of the pixel in the line senterY
        for(int j=0; j<W; j++) {
            line_buffer[j].rgb += imageIn->data[W*end_i+j].rgb;
        }
        horizontalAvg(imageOut, line_buffer, reciprocal, W, start_i, end_i, i, size);
    }
    // Middle part
    for(; i < H-size; i++) {
        end_i++;
        for(int j=0; j<W; j++) {
            line_buffer[j].rgb+=imageIn->data[W*end_i+j].rgb-imageIn->data[W*start_i+j].rgb;
        }
        start_i++;
        horizontalAvg(imageOut, line_buffer, reciprocal, W, start_i, end_i, i, size);
    }
    // End edge
    for(; i < H; i++) {
        for(int j=0; j<W; j++) {
            line_buffer[j].rgb -= imageIn->data[W*start_i+j].rgb;
        }
        start_i++;
        horizontalAvg(imageOut, line_buffer, reciprocal, W, start_i, end_i, i, size);
    }
}


/* Perform the final step, and save it as a ppm in imageOut.
 */
void convertBackAndWrite(AccurateImage *imageInSmall, AccurateImage *imageInLarge, PPMImage *imageOut, int argc, int size)
{
    imageOut->x = imageInSmall->x;
    imageOut->y = imageInSmall->y;
    for(int i = 0; i < imageInSmall->x * imageInSmall->y; i++)
    {
        v4f values = imageInLarge->data[i].rgb - imageInSmall->data[i].rgb;
        imageOut->data[i].red = (int) values[0];
        imageOut->data[i].green = (int) values[1];
        imageOut->data[i].blue = (int) values[2];
    }
    if(argc > 1) {
        if (size == 0)
            writePPM("flower_tiny.ppm", imageOut);
        else if (size == 1)
            writePPM("flower_small.ppm", imageOut);
        else
            writePPM("flower_medium.ppm", imageOut);
    } else {
        writeStreamPPM(stdout, imageOut);
    }
}

void blur5times(AccurateImage *imageNew, AccurateImage *imageUnchanged, AccurateImage *imageBuffer, int size)
{
    gaussianBlur(imageNew, imageUnchanged, size);
    gaussianBlur(imageBuffer, imageNew, size);
    gaussianBlur(imageNew, imageBuffer, size);
    gaussianBlur(imageBuffer, imageNew, size);
    gaussianBlur(imageNew, imageBuffer, size);
}


int main(int argc, char** argv)
{

    PPMImage *image;
    //AccuratePixel *line_buffer;

    if(argc > 1) {
        image = readPPM("flower.ppm");
    } else {
        image = readStreamPPM(stdin);
    }
    
    line_buffer = (AccuratePixel*)malloc(sizeof(AccuratePixel)*image->x);

    AccurateImage *imageUnchanged = convertImageToNewFormat(image); // save the unchanged image from input image
    AccurateImage *imageBuffer = createEmptyImage(image);
    AccurateImage *imageSmall = createEmptyImage(image);
    AccurateImage *imageBig = createEmptyImage(image);

    PPMImage *imageOut;
    imageOut = (PPMImage *)malloc(sizeof(PPMImage));
    imageOut->data = (PPMPixel*)malloc(image->x * image->y * sizeof(PPMPixel));

    // blur5times the tiny case:
    blur5times(imageSmall, imageUnchanged, imageBuffer, 2);

    // blur5times the small case:
    blur5times(imageBig, imageUnchanged, imageBuffer, 3);

    // save tiny case result
    convertBackAndWrite(imageSmall, imageBig, imageOut, argc, 0);

    // blur5times the medium case:
    blur5times(imageSmall, imageUnchanged, imageBuffer, 5);

    // save small case
    convertBackAndWrite(imageBig, imageSmall, imageOut, argc, 1);

    // blur5times the large case
    blur5times(imageBig, imageUnchanged, imageBuffer, 8);

    // save the medium case
    convertBackAndWrite(imageSmall, imageBig, imageOut, argc, 3);


    // free all memory structures
    freeImage(imageUnchanged);
    freeImage(imageBuffer);
    freeImage(imageSmall);
    freeImage(imageBig);
    free(imageOut->data);
    free(imageOut);
    free(image->data);
    free(image);
    free(line_buffer);

    return 0;
}
