
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
    v4f rg;
    v4f b;
} AccuratePixel;

typedef struct {
    int x, y;
    AccuratePixel *data; // TODO: this might require some changes for a SIMD version too.
} AccurateImage;

AccuratePixel *line_buffer;

// Convert ppm to high precision format.
AccurateImage *convertImageToNewFormat(PPMImage *image)
{
    int W = image->y;
    int H = image->x;

    if ( (image->x * image->y)%2 != 0) {
        printf("NOT div 2 ERROR\n");
    }
    // Make a copy
    AccurateImage *imageAccurate;
    imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));

    imageAccurate->data = (AccuratePixel*)malloc(sizeof(AccuratePixel) * (image->x*image->y)/2);

    for(int i = 0; i < H/2; i++) {
        for(int j = 0; j < W; j++) {
            v4f rg = {(float) image->data[i*W+j].red, 
                (float) image->data[i*W + j].green, 
                (float) image->data[(i+H/2)*W + j].red, 
                (float) image->data[(i+H/2)*W + j].green};
            imageAccurate->data[i].rg = rg;
            v4f b = {(float) image->data[i*W + j].blue, 
                (float) image->data[(i+H/2)*W + j].blue,
                0, 0};
            imageAccurate->data[i].b = b;
        }
    }
    imageAccurate->x = image->x;
    imageAccurate->y = image->y;

    return imageAccurate;
}

AccurateImage *createEmptyImage(PPMImage *image)
{
    AccurateImage *imageAccurate;
    imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));

    imageAccurate->data = (AccuratePixel*)malloc(sizeof(AccuratePixel) * (image->x*image->y)/2);

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

void horizontalAvg(AccurateImage *imageOut, AccuratePixel *line_buffer, float rec, int W, int starty, int endy, int senterY, int size)
{

    v4f sum_rg = {0, 0, 0, 0};
    v4f sum_b = {0, 0, 0, 0};
    // Initialization
    for (int x=0; x < size; x++){
        sum_rg += line_buffer[x].rg;
        sum_b += line_buffer[x].b;
    }

    // Start edge
    for(int i=0; i<=size; i++)
    {
        int endx = i+size;
        int startx = 0;
        sum_rg +=line_buffer[endx].rg;
        sum_b +=line_buffer[endx].b;
        rec = 1.0 / ((endx+1)*(endy-starty+1));
        imageOut->data[W * senterY + i].rg = sum_rg * rec;
        imageOut->data[W * senterY + i].b = sum_b * rec;
    }
    // Middle part
    for(int i=size+1; i<W-size; i++)
    {
        int startx = i-size;
        int endx = i+size;
        sum_rg += (line_buffer[endx].rg-line_buffer[startx-1].rg);
        sum_b += (line_buffer[endx].b-line_buffer[startx-1].b);
        imageOut->data[W * senterY + i].rg = sum_rg * rec;
        imageOut->data[W * senterY + i].b = sum_b * rec;
    }
    // End edge
    for(int i=W-size; i<W; i++)
    {
        int startx = i-size;
        int endx = W-1;
        sum_rg -= line_buffer[startx-1].rg;
        sum_b -= line_buffer[startx-1].b;
        rec = 1.0 / ((endx-startx+1)*(endy-starty+1));
        imageOut->data[W * senterY + i].rg = sum_rg * rec;
        imageOut->data[W * senterY + i].b = sum_b * rec;
    }
}

// Perform the new idea:
void performNewIdeaIteration(AccurateImage *imageOut, AccurateImage *imageIn, int size)
{
    float rec = 1.0 / (size * 2 + 1);


    int W = imageIn->x;
    int H = imageIn->y;

    int hs = size/2;
    int hw = W/2;
    int hh = H/2;
    // Line buffer for the vertical sums
    
    memset(line_buffer, 0, W * sizeof(AccuratePixel));

    int starty;
    int endy;

    // Some initialization
    for(int line_y=0; line_y < size; line_y++){
        for(int i=0; i<hw; i++){
            line_buffer[i].rg += imageIn->data[hw*line_y+i].rg;
            line_buffer[i].b += imageIn->data[hw*line_y+i].b;
        }
    }
    // Start edge
    for(int senterY = 0; senterY <= size; senterY++) {
        // first and last line considered  by the computation of the pixel in the line senterY
        endy = senterY+size;
        starty = 0;
        for(int i=0; i<hw; i++) {
            line_buffer[i].rg += imageIn->data[hw*endy+i].rg;
            line_buffer[i].b += imageIn->data[hw*endy+i].b;
        }
        horizontalAvg(imageOut, line_buffer, rec, hw, starty, endy, senterY, size);
    }
    // Middle part
    for(int senterY = size+1; senterY < hh-size; senterY++) {
        starty = senterY-size;
        endy = senterY+size;
        for(int i=0; i<hw; i++) {
            line_buffer[i].rg+=imageIn->data[hw*endy+i].rg-imageIn->data[hw*(starty-1)+i].rg;
            line_buffer[i].b+=imageIn->data[hw*endy+i].b-imageIn->data[hw*(starty-1)+i].b;
        }
        horizontalAvg(imageOut, line_buffer, rec, hw, starty, endy, senterY, size);
    }
    // End edge
    endy = H-1;
    for(int senterY = hh-size; senterY < hh; senterY++) {
        starty = senterY-size;
        for(int i=0; i<hw; i++) {
            line_buffer[i].rg -= imageIn->data[hw*(starty-1)+i].rg;
            line_buffer[i].b -= imageIn->data[hw*(starty-1)+i].b;
        }
        horizontalAvg(imageOut, line_buffer, rec, hw, starty, endy, senterY, size);
    }
}


// Perform the final step, and save it as a ppm in imageOut
void performNewIdeaFinalization(AccurateImage *imageInSmall, AccurateImage *imageInLarge, PPMImage *imageOut, int argc, int size)
{
    int H = imageInSmall->x;
    int W = imageInSmall->y;
    imageOut->x = W;
    imageOut->y = H;

    for(int i = 0; i < H/2; i++)
    {
        for(int j = 0; j < W; j++)
        {
            v4f rg_values = imageInLarge->data[i*W + j].rg - imageInSmall->data[i*W + j].rg;
            v4f b_values = imageInLarge->data[i*W + j].b - imageInSmall->data[i*W + j].b;
            imageOut->data[i*W + j].red = (int) rg_values[0];
            imageOut->data[i*W + j].green = (int) rg_values[1];
            imageOut->data[i*W + j].blue = (int) b_values[0];
            imageOut->data[(i+H/2)*W + j].red = (int) rg_values[2];
            imageOut->data[(i+H/2)*W + j].green = (int) rg_values[3];
            imageOut->data[(i+H/2)*W + j].blue = (int) b_values[1];
        }
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

    line_buffer = (AccuratePixel*)malloc(sizeof(AccuratePixel)*image->x*image->y/2);
    

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
