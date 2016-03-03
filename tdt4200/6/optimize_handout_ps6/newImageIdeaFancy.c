

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
} AccuratePixelRG;

typedef struct {
    v4f b;
} AccuratePixelB;

typedef struct {
    int x, y;
    AccuratePixel *rg_data;
    AccuratePixel *b_data;
} AccurateImage;

AccuratePixel *line_buffer;

// Convert ppm to high precision format.
AccurateImage *convertImageToNewFormat(PPMImage *image)
{

    int half = (image->x * image->y)/2;
    // Make a copy
    AccurateImage *imageAccurate;
    imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));
    if ((image->x * image->y)%2 != 0) {
        printf("NOT DIV 2 ERROR\n");
    }
    if ((image->x * image->y)%4 != 0) {
        printf("NOT DIV 4 ERROR\n");
    }
    imageAccurate->rg_data = (AccuratePixelRG*)malloc(sizeof(AccuratePixelRG) * (image->x * image->y / 2));
    imageAccurate->b_data = (AccuratePixelRG*)malloc(sizeof(AccuratePixelRG) * (image->x * image->y / 4));
    //posix_memalign((void *)&(imageAccurate->data), sizeof(v4f), image->x * image->y * sizeof(AccuratePixel));
    for(int i = 0; i < half; i++) {
        v4f rg = {(float) image->data[i].red, 
            (float) image->data[i].green, 
            (float) image->data[2*i].red, 
            (float) image->data[2*i].green};
        imageAccurate->rg_data[i].rg = rg;
        if (i < half / 2) {
            v4f b = {(float) image->data[i].blue, 
                (float) image->data[2*i].blue, 
                (float) image->data[3*i].blue, 
                (float) image->data[4*i].blue};
            imageAccurate->b_data[i].b = b;
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

    imageAccurate->rg_data = (AccuratePixelRG*)malloc(sizeof(AccuratePixelRG) * (image->x * image->y / 2));
    imageAccurate->b_data = (AccuratePixelRG*)malloc(sizeof(AccuratePixelRG) * (image->x * image->y / 4));

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
        sum_rgb +=line_buffer[endx].rgb;
        rec = 1.0 / ((endx+1)*(endy-starty+1));
        imageOut->data[W * senterY + i].rgb = sum_rgb * rec;
    }
    // Middle part
    for(int i=size+1; i<W-size; i++)
    {
        int startx = i-size;
        int endx = i+size;
        sum_rgb += (line_buffer[endx].rgb-line_buffer[startx-1].rgb);
        imageOut->data[W * senterY + i].rgb = sum_rgb * rec;
    }
    // End edge
    for(int i=W-size; i<W; i++)
    {
        int startx = i-size;
        int endx = W-1;
        sum_rgb -= line_buffer[startx-1].rgb;
        rec = 1.0 / ((endx-startx+1)*(endy-starty+1));
        imageOut->data[W * senterY + i].rgb = sum_rgb * rec;
    }
}

// Perform the new idea:
void performNewIdeaIteration(AccurateImage *imageOut, AccurateImage *imageIn, int size)
{
    float rec = 1.0 / (size * 2 + 1);

    int W = imageIn->x;
    int H = imageIn->y;

    // Line buffer for the vertical sums
    
    memset(line_buffer, 0, W * sizeof(AccuratePixel));

    int starty;
    int endy;

    // Some initialization
    for(int line_y=0; line_y < size; line_y++){
        for(int i=0; i<W; i++){
            line_buffer[i].rgb += imageIn->data[W*line_y+i].rgb;
        }
    }
    // Start edge
    for(int senterY = 0; senterY <= size; senterY++) {
        // first and last line considered  by the computation of the pixel in the line senterY
        endy = senterY+size;
        starty = 0;
        for(int i=0; i<W; i++) {
            line_buffer[i].rgb += imageIn->data[W*endy+i].rgb;
        }
        horizontalAvg(imageOut, line_buffer, rec, W, starty, endy, senterY, size);
    }
    // Middle part
    for(int senterY = size+1; senterY < H-size; senterY++) {
        starty = senterY-size;
        endy = senterY+size;
        for(int i=0; i<W; i++) {
            line_buffer[i].rgb+=imageIn->data[W*endy+i].rgb-imageIn->data[W*(starty-1)+i].rgb;
        }
        horizontalAvg(imageOut, line_buffer, rec, W, starty, endy, senterY, size);
    }
    // End edge
    endy = H-1;
    for(int senterY = H-size; senterY < H; senterY++) {
        starty = senterY-size;
        for(int i=0; i<W; i++) {
            line_buffer[i].rgb -= imageIn->data[W*(starty-1)+i].rgb;
        }
        horizontalAvg(imageOut, line_buffer, rec, W, starty, endy, senterY, size);
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
        imageOut->data[i].red = values[0];
        imageOut->data[i].green = values[1];
        imageOut->data[i].blue = values[2];
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
