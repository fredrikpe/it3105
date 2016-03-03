#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "ppm.h"

//#pragma GCC optimize("Ofast")

// Image from:
// http://7-themes.com/6971875-funny-flowers-pictures.html


typedef struct {
    float red,green,blue;
} AccuratePixel;

typedef struct {
    int x, y;
    AccuratePixel *data;
} AccurateImage;


// Convert ppm to high precision format.
static AccurateImage *convertImageToNewFormat(PPMImage *image) {
    AccurateImage *aImg;
    aImg = (AccurateImage *)malloc(sizeof(AccurateImage));
    aImg->data = (AccuratePixel*)malloc(image->x * image->y * sizeof(AccuratePixel));
#pragma omp parallel for
    for(int i = 0; i < image->x * image->y; i++) {
        aImg->data[i].red   = (float) image->data[i].red;
        aImg->data[i].green = (float) image->data[i].green;
        aImg->data[i].blue  = (float) image->data[i].blue;
    }
    aImg->x = image->x;
    aImg->y = image->y;

    return aImg;
}

static AccurateImage *createEmptyImage(PPMImage *image)
{
    AccurateImage *aImg;
    aImg = (AccurateImage *)malloc(sizeof(AccurateImage));
    aImg->data = (AccuratePixel*)malloc(image->x * image->y * sizeof(AccuratePixel));
    aImg->x = image->x;
    aImg->y = image->y;

    return aImg;
}

// free memory of an AccurateImage
static void freeImage(AccurateImage *image)
{
    free(image->data);
    free(image);
}

static void gaussBlur(AccurateImage *imageOut,
                      AccurateImage *imageIn,
                      int size) 
{
    // initialize openMP constants used for parallelizing.
    int myRank = omp_get_thread_num(); 
    int ompSize = omp_get_num_threads(); 

    int myStart = myRank * (imageIn->y /ompSize);
    int myEnd = myStart + (imageIn->y / ompSize);
    if (myRank == ompSize - 1){
        myEnd = imageIn->y;
    }

    int offsetOfThePixel=0;
    float sum_red = 0;
    float sum_blue = 0;
    float sum_green =0;

    int W = imageIn->x;
    int H = imageIn->y;

    float rec = 1.0 / (size * 2 + 1);

    AccuratePixel *line_buffer;
    line_buffer = (AccuratePixel*)malloc(sizeof(AccuratePixel)*W);

    memset(line_buffer,0,W*sizeof(AccuratePixel)); 

    // Iterate over image height
    for(int i = myStart; i < myEnd; i++) {

        int start_i = i-size;
        int end_i = i+size;

        if (i == myStart){
            if (start_i <= 0){
                start_i = 0;
            }
            for(int line_y=start_i; line_y < end_i+1; line_y++){
                for(int j=0; j<W; j++){
                    line_buffer[j].blue+=imageIn->data[W*line_y+j].blue;
                    line_buffer[j].red+=imageIn->data[W*line_y+j].red;
                    line_buffer[j].green+=imageIn->data[W*line_y+j].green;
                }
            }
        } else if (start_i <=0){
            start_i = 0;
            for(int j=0; j<W; j++){
                line_buffer[j].blue+=imageIn->data[W*end_i+j].blue;
                line_buffer[j].red+=imageIn->data[W*end_i+j].red;
                line_buffer[j].green+=imageIn->data[W*end_i+j].green;
            }

        } else if (end_i >= H ){
            end_i = H - 1;
            for(int j=0; j<W; j++){
                line_buffer[j].blue-=imageIn->data[W*(start_i-1)+j].blue;
                line_buffer[j].red-=imageIn->data[W*(start_i-1)+j].red;
                line_buffer[j].green-=imageIn->data[W*(start_i-1)+j].green;
            }   
        } else {
            for(int j=0; j<W; j++){
                line_buffer[j].blue += imageIn->data[W*end_i+j].blue
                    - imageIn->data[W*(start_i-1)+j].blue;
                line_buffer[j].red += imageIn->data[W*end_i+j].red
                    - imageIn->data[W*(start_i-1)+j].red;
                line_buffer[j].green += imageIn->data[W*end_i+j].green
                    - imageIn->data[W*(start_i-1)+j].green;
            }   
        }
        // End of line_buffer adjustment.

        sum_green = 0;
        sum_red = 0;
        sum_blue = 0;

        int del1 = end_i-start_i + 1;

        float rec = 1.0 / (size*2 + 1);
        

        int start_j = 0;
        int end_j = size;
        for(int j = 0; j < W; j++) {
            if (j <= size){
                if(j==0){
                    for (int x=0; x < size; x++){
                        sum_red += line_buffer[x].red;
                        sum_green += line_buffer[x].green;
                        sum_blue += line_buffer[x].blue;
                    }
                }
                sum_red +=line_buffer[end_j].red;
                sum_green +=line_buffer[end_j].green;
                sum_blue +=line_buffer[end_j].blue;
                rec = 1.0/((end_j-start_j+1)*del1);
                end_j++;
            }else if (end_j >= W){
                start_j++;
                sum_red -=line_buffer[start_j-1].red;
                sum_green -=line_buffer[start_j-1].green;
                sum_blue -=line_buffer[start_j-1].blue;
                rec = 1.0/((W-1-start_j+1)*del1);
            }else{
                start_j++;
                sum_red += (line_buffer[end_j].red-line_buffer[start_j-1].red);
                sum_green += (line_buffer[end_j].green-line_buffer[start_j-1].green);
                sum_blue += (line_buffer[end_j].blue-line_buffer[start_j-1].blue);
                end_j++;
            }           

            // we save each pixel in the output image
            offsetOfThePixel = (W * i + j);
            imageOut->data[offsetOfThePixel].red = sum_red*rec;
            imageOut->data[offsetOfThePixel].green = sum_green*rec;
            imageOut->data[offsetOfThePixel].blue = sum_blue*rec;
        }

    }
    free(line_buffer);
#pragma omp barrier
}

// Perform the final step, and save it as a ppm in imageOut
static void convertBackToPPM(AccurateImage *imageInSmall, 
        AccurateImage *imageInLarge, 
        PPMImage *imageOut) 
{
    imageOut->x = imageInSmall->x;
    imageOut->y = imageInSmall->y;

#pragma omp for schedule(static)
    for(int i = 0; i < imageInSmall->x * imageInSmall->y; i++) {
        float value = (imageInLarge->data[i].red - imageInSmall->data[i].red);
        imageOut->data[i].red = value;

        value = (imageInLarge->data[i].green - imageInSmall->data[i].green);
        imageOut->data[i].green = value;

        value = (imageInLarge->data[i].blue - imageInSmall->data[i].blue);
        imageOut->data[i].blue = value;
    }
}

static void blur5times(AccurateImage *in, 
                     AccurateImage *out, 
                     AccurateImage *buffer, 
                     int size)
{
    gaussBlur(in, out, size);
    gaussBlur(buffer, in, size);
    gaussBlur(in, buffer, size);
    gaussBlur(buffer, in, size);
    gaussBlur(in, buffer, size);
}



int main(int argc, char** argv) {

    PPMImage *image;

    if(argc > 1) {
        image = readPPM("flower.ppm");
    } else {
        image = readStreamPPM(stdin);
    }


    PPMImage *imageOut;
    imageOut = (PPMImage *)malloc(sizeof(PPMImage));
    imageOut->data = (PPMPixel*)malloc(image->x * image->y * sizeof(PPMPixel));

    AccurateImage *imageUnchanged = convertImageToNewFormat(image); 
    AccurateImage *imageBuffer = createEmptyImage(image);
    AccurateImage *imageSmall = createEmptyImage(image);
    AccurateImage *imageBig = createEmptyImage(image);

#pragma omp parallel 
    {
        // Process the tiny case:
        blur5times(imageSmall, imageUnchanged, imageBuffer, 2);

        // Process the small case:
        blur5times(imageBig, imageUnchanged, imageBuffer,3);

        // save tiny case result
        convertBackToPPM(imageSmall,  imageBig, imageOut);
    }

    if(argc > 1) {
        writePPM("flower_tiny.ppm", imageOut);
    } else {
        writeStreamPPM(stdout, imageOut);
    }

#pragma omp parallel
    {
        // Process the medium case:
        blur5times(imageSmall, imageUnchanged, imageBuffer, 5);

        // save small case
        convertBackToPPM(imageBig,  imageSmall,imageOut);
    }

    if(argc > 1) {
        writePPM("flower_small.ppm", imageOut);
    } else {
        writeStreamPPM(stdout, imageOut);
    }

#pragma omp parallel
    {
        // process the large case
        blur5times(imageBig, imageUnchanged, imageBuffer, 8);

        // save the medium case
        convertBackToPPM(imageSmall,  imageBig, imageOut);
    }

    if(argc > 1) {
        writePPM("flower_medium.ppm", imageOut);
    } else {
        writeStreamPPM(stdout, imageOut);
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

