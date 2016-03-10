
/* Image processing with simd instructions and omp.
 *
 * Calculates the difference of gassians (DoG, an algorithm
 * used for image feature enhancement) beetween two images convoluted 5 
 * times with a gaussian blur in different sizes.
 */
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

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


// Convert ppm to high precision format.
AccurateImage *convertImageToNewFormat(PPMImage *image)
{
    // Make a copy
    AccurateImage *imageAccurate;
    imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));

    imageAccurate->data = (AccuratePixel*)malloc(sizeof(AccuratePixel)*image->x*image->y);

#pragma omp parallel for 
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

void gaussianBlur(AccurateImage *imageOut, AccurateImage *imageIn, int size)
{
    float reciprocal = 1.0 / (size * 2 + 1);

    int W = imageIn->x;
    int H = imageIn->y;
    
    // initialize openMP constants used for parallelizing.
    int myRank = omp_get_thread_num(); 
    int ompSize = omp_get_num_threads(); 


    int myStart = myRank * (imageIn->y /ompSize);
    int myEnd = myStart + (imageIn->y / ompSize);
    if (myRank == ompSize - 1){
        myEnd = H;
    }

    // line buffer that will save the sum of some pixel in the column
    AccuratePixel *line_buffer = (AccuratePixel*) malloc(imageIn->x*sizeof(AccuratePixel));
    memset(line_buffer,0,imageIn->x*sizeof(AccuratePixel)); 

    // Iterate over each line of pixelx.
    for(int senterY = myStart; senterY < myEnd; senterY++) {
        int starty = senterY-size;
        int endy = senterY+size;

        if (senterY == myStart){
            if (starty <= 0){
                starty = 0;
            }
            for(int line_y=starty; line_y < endy+1; line_y++){
                for(int i=0; i<imageIn->x; i++){
                    line_buffer[i].rgb+=imageIn->data[W*line_y+i].rgb;
                }
            }
        } else if (starty <=0){
            starty = 0;
            for(int i=0; i<imageIn->x; i++){
                // add the next pixel of the next line in the column x
                line_buffer[i].rgb+=imageIn->data[W*endy+i].rgb;
            }

        } else if (endy >= imageIn->y ){
            // for the last lines, we just need to subtract the first added line
            endy = imageIn->y-1;
            for(int i=0; i<imageIn->x; i++){
                line_buffer[i].rgb-=imageIn->data[W*(starty-1)+i].rgb;
            }   
        } else {
            for(int i=0; i<imageIn->x; i++){
                line_buffer[i].rgb+=imageIn->data[W*endy+i].rgb-imageIn->data[W*(starty-1)+i].rgb;
            }   
        }
        horizontalAvg(imageOut, line_buffer, reciprocal, W, starty, endy, senterY, size);
    }
    free(line_buffer);
#pragma omp barrier
}


/* Perform the final step, and save it as a ppm in imageOut.
 */
void convertBackAndWrite(AccurateImage *imageInSmall, AccurateImage *imageInLarge, PPMImage *imageOut, int argc, int size)
{
    imageOut->x = imageInSmall->x;
    imageOut->y = imageInSmall->y;
#pragma omp for schedule(static)
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
    
    AccurateImage *imageUnchanged = convertImageToNewFormat(image);
    AccurateImage *imageBuffer = createEmptyImage(image);
    AccurateImage *imageSmall = createEmptyImage(image);
    AccurateImage *imageBig = createEmptyImage(image);

    PPMImage *imageOut;
    imageOut = (PPMImage *)malloc(sizeof(PPMImage));
    imageOut->data = (PPMPixel*)malloc(image->x * image->y * sizeof(PPMPixel));

#pragma omp parallel
    {
        // blur5times the tiny case:
        blur5times(imageSmall, imageUnchanged, imageBuffer, 2);

        // blur5times the small case:
        blur5times(imageBig, imageUnchanged, imageBuffer, 3);

        // save tiny case result
        convertBackAndWrite(imageSmall, imageBig, imageOut, argc, 0);
    }

#pragma omp parallel
    {
        // blur5times the medium case:
        blur5times(imageSmall, imageUnchanged, imageBuffer, 5);

        // save small case
        convertBackAndWrite(imageBig, imageSmall, imageOut, argc, 1);
    }

#pragma omp parallel
    {
        // blur5times the large case
        blur5times(imageBig, imageUnchanged, imageBuffer, 8);

        // save the medium case
        convertBackAndWrite(imageSmall, imageBig, imageOut, argc, 3);
    }

    // free all memory structures
    freeImage(imageUnchanged);
    freeImage(imageBuffer);
    freeImage(imageSmall);
    freeImage(imageBig);
    free(imageOut->data);
    free(imageOut);
    free(image->data);
    free(image);

    return 0;
}
