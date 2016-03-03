#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "ppm.h"


// Image from:
// http://7-themes.com/6971875-funny-flowers-pictures.html


typedef struct {
    float red,green,blue;
} AccuratePixel;

AccuratePixel *line_buffer;

typedef struct {
    int x, y;
    AccuratePixel *data;
} AccurateImage;

// Convert ppm to high precision format.
static AccurateImage *convertImageToNewFormat(PPMImage *image) {
    // Make a copy
    AccurateImage *imageAccurate;
    imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));
    imageAccurate->data = (AccuratePixel*)malloc(image->x * image->y * sizeof(AccuratePixel));
#pragma omp parallel for
    for(int i = 0; i < image->x * image->y; i++) {
        imageAccurate->data[i].red   = (float) image->data[i].red;
        imageAccurate->data[i].green = (float) image->data[i].green;
        imageAccurate->data[i].blue  = (float) image->data[i].blue;
    }
    imageAccurate->x = image->x;
    imageAccurate->y = image->y;

    return imageAccurate;
}

static AccurateImage *createEmptyImage(PPMImage *image)
{
    AccurateImage *imageAccurate;
    imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));
    imageAccurate->data = (AccuratePixel*)malloc(image->x * image->y * sizeof(AccuratePixel));
    imageAccurate->x = image->x;
    imageAccurate->y = image->y;

    return imageAccurate;
}

// free memory of an AccurateImage
static void freeImage(AccurateImage *image)
{
    free(image->data);
    free(image);
}

static void performNewIdeaIteration(AccurateImage *imageOut, AccurateImage *imageIn,int size)
{    
    float rec = 1.0 / (size * 2 + 1);

    int W = imageIn->x;
    int H = imageIn->y;

    int myRank = omp_get_thread_num(); 
    int ompSize = omp_get_num_threads(); 

    float sum_red, sum_blue, sum_green;

    int myStart = myRank * (imageIn->y /ompSize);
    int myEnd = myStart + (imageIn->y / ompSize);
    if (myRank == ompSize - 1){
        myEnd = imageIn->y;
    }
    int countIncluded = 0;
    int offsetOfThePixel=0;
    int numberOfValuesInEachRow = imageIn->x;


    memset(line_buffer,0,imageIn->x*sizeof(AccuratePixel)); 

    for(int i = myStart; i < myEnd; i++) {
        int start_i = i-size;
        int end_i = i+size;

        if (i == myStart){
            if (start_i <= 0){
                start_i = 0;
            }
            for(int j=start_i; j < end_i+1; j++){
                for(int i=0; i<imageIn->x; i++){
                    line_buffer[i].blue+=imageIn->data[W*j+i].blue;
                    line_buffer[i].red+=imageIn->data[W*j+i].red;
                    line_buffer[i].green+=imageIn->data[W*j+i].green;
                }
            }
        } else if (start_i <=0){
            start_i = 0;
            for(int j=0; j<imageIn->x; j++){
                line_buffer[j].blue+=imageIn->data[W*end_i+j].blue;
                line_buffer[j].red+=imageIn->data[W*end_i+j].red;
                line_buffer[j].green+=imageIn->data[W*end_i+j].green;
            }

        } else if (end_i >= imageIn->y ){
            end_i = H-1;
            for(int j=0; j<imageIn->x; j++){
                line_buffer[j].blue-=imageIn->data[W*(start_i-1)+j].blue;
                line_buffer[j].red-=imageIn->data[W*(start_i-1)+j].red;
                line_buffer[j].green-=imageIn->data[W*(start_i-1)+j].green;
            }   
        } else {
            for(int j=0; j<imageIn->x; j++){
                line_buffer[j].blue+=imageIn->data[W*end_i+j].blue-imageIn->data[W*(start_i-1)+j].blue;
                line_buffer[j].red+=imageIn->data[W*end_i+j].red-imageIn->data[W*(start_i-1)+j].red;
                line_buffer[j].green+=imageIn->data[W*end_i+j].green-imageIn->data[W*(start_i-1)+j].green;
            }   
        }

        sum_green =0;
        sum_red = 0;
        sum_blue = 0;
        for(int senterX = 0; senterX < imageIn->x; senterX++) {
            int startx = senterX-size;
            int endx = senterX+size;

            if (startx <=0){
                startx = 0;
                if(senterX==0){
                    for (int x=startx; x < endx; x++){
                        sum_red += line_buffer[x].red;
                        sum_green += line_buffer[x].green;
                        sum_blue += line_buffer[x].blue;
                    }
                }
                sum_red +=line_buffer[endx].red;
                sum_green +=line_buffer[endx].green;
                sum_blue +=line_buffer[endx].blue;
            }else if (endx >= imageIn->x){
                endx = imageIn->x-1;
                sum_red -=line_buffer[startx-1].red;
                sum_green -=line_buffer[startx-1].green;
                sum_blue -=line_buffer[startx-1].blue;

            }else{
                sum_red += (line_buffer[endx].red-line_buffer[startx-1].red);
                sum_green += (line_buffer[endx].green-line_buffer[startx-1].green);
                sum_blue += (line_buffer[endx].blue-line_buffer[startx-1].blue);
            }           

            // we save each pixel in the output image
            offsetOfThePixel = (numberOfValuesInEachRow * i + senterX);
            countIncluded=(endx-startx+1)*(end_i-start_i+1);

            imageOut->data[offsetOfThePixel].red = sum_red/countIncluded;
            imageOut->data[offsetOfThePixel].green = sum_green/countIncluded;
            imageOut->data[offsetOfThePixel].blue = sum_blue/countIncluded;
        }
    }
#pragma omb barrier 
}

// Perform the final step, and save it as a ppm in imageOut
void performNewIdeaFinalization(AccurateImage *imageInSmall, AccurateImage *imageInLarge, PPMImage *imageOut, int argc, int size)
{
    imageOut->x = imageInSmall->x;
    imageOut->y = imageInSmall->y;
#pragma omp for schedule(static)
    for(int i=0; i<imageInSmall->x * imageInSmall->y; i++)
    {
        float value = imageInLarge->data[i].red - imageInSmall->data[i].red;
        imageOut->data[i].red = value;
        value = imageInLarge->data[i].green - imageInSmall->data[i].green;
        imageOut->data[i].green = value;
        value = imageInLarge->data[i].blue - imageInSmall->data[i].blue;
        imageOut->data[i].blue = value;
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

    if(argc > 1) {
        image = readPPM("flower.ppm");
    } else {
        image = readStreamPPM(stdin);
    }
    
    line_buffer = (AccuratePixel*)malloc(sizeof(AccuratePixel) * image->x * image->y);

    AccurateImage *imageUnchanged = convertImageToNewFormat(image); 
    AccurateImage *imageBuffer = createEmptyImage(image);
    AccurateImage *imageSmall = createEmptyImage(image);
    AccurateImage *imageBig = createEmptyImage(image);

    PPMImage *imageOut;
    imageOut = (PPMImage *)malloc(sizeof(PPMImage));
    imageOut->data = (PPMPixel*)malloc(image->x * image->y * sizeof(PPMPixel));

#pragma omp parallel
    {
        // Process the tiny case:
        process(imageSmall, imageUnchanged, imageBuffer, 2);

        // Process the small case:
        process(imageBig, imageUnchanged, imageBuffer, 3);

        // save tiny case result
        performNewIdeaFinalization(imageSmall, imageBig, imageOut, argc, 0);
    }

#pragma omp parallel
    {
        // Process the medium case:
        process(imageSmall, imageUnchanged, imageBuffer, 5);

        // save small case
        performNewIdeaFinalization(imageBig, imageSmall, imageOut, argc, 1);
    }
#pragma omp parallel
    {
        // process the large case
        process(imageBig, imageUnchanged, imageBuffer, 8);

        // save the medium case
        performNewIdeaFinalization(imageSmall, imageBig, imageOut, argc, 3);
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
    free(line_buffer);

    return 0;
}
