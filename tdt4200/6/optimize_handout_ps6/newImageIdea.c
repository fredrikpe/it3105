
#define _POSIX_C_SOURCE 200809L

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ppm.h"


// Image from:
// http://7-themes.com/6971875-funny-flowers-pictures.html

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

    posix_memalign((void *)&(imageAccurate->data), 
                sizeof(v4f), 
                image->x * image->y * sizeof(AccuratePixel));
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
    posix_memalign((void *)&(imageAccurate->data), 
                sizeof(v4f), 
                image->x * image->y * sizeof(AccuratePixel));
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

void horizontalAvg(AccurateImage *imageOut, AccuratePixel *line_buffer, float rec, int W, int start_i, int end_i, int i, int size)
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
    for(j=0; j<=size; j++)
    {
        end_j++;
        sum_rgb +=line_buffer[end_j].rgb;
        rec = 1.0 / ((end_j+1)*(end_i-start_i+1));
        imageOut->data[W*i + j].rgb = sum_rgb * rec;
    }
    // Middle part
    for(;j<W-size; j++)
    {
        start_j++;
        end_j++;
        sum_rgb += line_buffer[end_j].rgb-line_buffer[start_j-1].rgb;
        imageOut->data[W*i + j].rgb = sum_rgb * rec;
    }
    // End edge
    for(;j<W; j++)
    {
        start_j++;
        sum_rgb -= line_buffer[start_j-1].rgb;
        rec = 1.0 / ((end_j-start_j+1)*(end_i-start_i+1));
        imageOut->data[W*i + j].rgb = sum_rgb * rec;
    }
}

// Perform the new idea:
void performNewIdeaIteration(AccurateImage *imageOut, AccurateImage *imageIn, int size)
{
    float rec = 1.0 / (size * 2 + 1);

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
        horizontalAvg(imageOut, line_buffer, rec, W, start_i, end_i, i, size);
    }
    // Middle part
    for(; i < H-size; i++) {
        end_i++;
        for(int j=0; j<W; j++) {
            line_buffer[j].rgb+=imageIn->data[W*end_i+j].rgb-imageIn->data[W*start_i+j].rgb;
        }
        start_i++;
        horizontalAvg(imageOut, line_buffer, rec, W, start_i, end_i, i, size);
    }
    // End edge
    for(; i < H; i++) {
        for(int j=0; j<W; j++) {
            line_buffer[j].rgb -= imageIn->data[W*start_i+j].rgb;
        }
        start_i++;
        horizontalAvg(imageOut, line_buffer, rec, W, start_i, end_i, i, size);
    }
}


// Perform the final step, and save it as a ppm in imageOut
void performNewIdeaFinalization(AccurateImage *imageInSmall, AccurateImage *imageInLarge, PPMImage *imageOut, int argc, int size)
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

void process(AccurateImage *imageNew, AccurateImage *imageUnchanged, AccurateImage *imageBuffer, int size)
{
    performNewIdeaIteration(imageNew, imageUnchanged, size);
    performNewIdeaIteration(imageBuffer, imageNew, size);
    performNewIdeaIteration(imageNew, imageBuffer, size);
    performNewIdeaIteration(imageBuffer, imageNew, size);
    performNewIdeaIteration(imageNew, imageBuffer, size);
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
    
    posix_memalign((void *)&(line_buffer), 
                sizeof(v4f), 
                image->x * sizeof(AccuratePixel));

    AccurateImage *imageUnchanged = convertImageToNewFormat(image); // save the unchanged image from input image
    AccurateImage *imageBuffer = createEmptyImage(image);
    AccurateImage *imageSmall = createEmptyImage(image);
    AccurateImage *imageBig = createEmptyImage(image);

    PPMImage *imageOut;
    imageOut = (PPMImage *)malloc(sizeof(PPMImage));
    imageOut->data = (PPMPixel*)malloc(image->x * image->y * sizeof(PPMPixel));

    // Process the tiny case:
    process(imageSmall, imageUnchanged, imageBuffer, 2);

    // Process the small case:
    process(imageBig, imageUnchanged, imageBuffer, 3);

    // save tiny case result
    performNewIdeaFinalization(imageSmall, imageBig, imageOut, argc, 0);

    // Process the medium case:
    process(imageSmall, imageUnchanged, imageBuffer, 5);

    // save small case
    performNewIdeaFinalization(imageBig, imageSmall, imageOut, argc, 1);

    // process the large case
    process(imageBig, imageUnchanged, imageBuffer, 8);

    // save the medium case
    performNewIdeaFinalization(imageSmall, imageBig, imageOut, argc, 3);


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
