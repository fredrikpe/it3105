#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "ppm.h"

#pragma GCC optimize("Ofast")

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
AccurateImage *convertImageToNewFormat(PPMImage *image) {
    // Make a copy
    AccurateImage *imageAccurate;
    imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));
    imageAccurate->data = (AccuratePixel*)malloc(image->x * image->y * sizeof(AccuratePixel));
    // CHANGE: Added parallel for section here.
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

AccurateImage *createEmptyImage(PPMImage *image){
    AccurateImage *imageAccurate;
    imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));
    imageAccurate->data = (AccuratePixel*)malloc(image->x * image->y * sizeof(AccuratePixel));
    imageAccurate->x = image->x;
    imageAccurate->y = image->y;

    return imageAccurate;
}

// free memory of an AccurateImage
void freeImage(AccurateImage *image){
    free(image->data);
    free(image);
}

void performNewIdeaIteration(AccurateImage *imageOut, AccurateImage *imageIn,int size) {
    // initialize openMP constants used for parallelizing.
    int myRank = omp_get_thread_num(); 
    int ompSize = omp_get_num_threads(); 

    // int myRank = 0; 
    // int ompSize = 1; 


    int myStart = myRank * (imageIn->y /ompSize);
    int myEnd = myStart + (imageIn->y / ompSize);
    if (myRank == ompSize - 1){
        myEnd = imageIn->y;
    }

    int countIncluded = 0;
    int offsetOfThePixel=0;
    float sum_red = 0;
    float sum_blue = 0;
    float sum_green =0;
    int numberOfValuesInEachRow = imageIn->x;

    // line buffer that will save the sum of some pixel in the column
    AccuratePixel *line_buffer = (AccuratePixel*) malloc(imageIn->x*sizeof(AccuratePixel));
    memset(line_buffer,0,imageIn->x*sizeof(AccuratePixel)); 


    // Iterate over each line of pixelx.
    for(int senterY = myStart; senterY < myEnd; senterY++) {
        // first and last line considered  by the computation of the pixel in the line senterY
        int starty = senterY-size;
        int endy = senterY+size;

        // CHANGE: restructured initialization of linebuffer.
        // init brand new linebuffer
        if (senterY == myStart){
            if (starty <= 0){
                starty = 0;
            }
            for(int line_y=starty; line_y < endy+1; line_y++){
                for(int i=0; i<imageIn->x; i++){
                    line_buffer[i].blue+=imageIn->data[numberOfValuesInEachRow*line_y+i].blue;
                    line_buffer[i].red+=imageIn->data[numberOfValuesInEachRow*line_y+i].red;
                    line_buffer[i].green+=imageIn->data[numberOfValuesInEachRow*line_y+i].green;
                }
            }
        }


        // start of adjusting linebuffer for new senterY

        else if (starty <=0){
            starty = 0;
            for(int i=0; i<imageIn->x; i++){
                // add the next pixel of the next line in the column x
                line_buffer[i].blue+=imageIn->data[numberOfValuesInEachRow*endy+i].blue;
                line_buffer[i].red+=imageIn->data[numberOfValuesInEachRow*endy+i].red;
                line_buffer[i].green+=imageIn->data[numberOfValuesInEachRow*endy+i].green;
            }

        }

        else if (endy >= imageIn->y ){
            // for the last lines, we just need to subtract the first added line
            endy = imageIn->y-1;
            for(int i=0; i<imageIn->x; i++){
                line_buffer[i].blue-=imageIn->data[numberOfValuesInEachRow*(starty-1)+i].blue;
                line_buffer[i].red-=imageIn->data[numberOfValuesInEachRow*(starty-1)+i].red;
                line_buffer[i].green-=imageIn->data[numberOfValuesInEachRow*(starty-1)+i].green;
            }   
        }else{
            // general case
            // add the next line and remove the first added line
            for(int i=0; i<imageIn->x; i++){
                line_buffer[i].blue+=imageIn->data[numberOfValuesInEachRow*endy+i].blue-imageIn->data[numberOfValuesInEachRow*(starty-1)+i].blue;
                line_buffer[i].red+=imageIn->data[numberOfValuesInEachRow*endy+i].red-imageIn->data[numberOfValuesInEachRow*(starty-1)+i].red;
                line_buffer[i].green+=imageIn->data[numberOfValuesInEachRow*endy+i].green-imageIn->data[numberOfValuesInEachRow*(starty-1)+i].green;
            }   
        }
        // End of line_buffer adjustment.


        sum_green =0;
        sum_red = 0;
        sum_blue = 0;
        for(int senterX = 0; senterX < imageIn->x; senterX++) {
            // in this loop, we do exactly the same thing as before but only with the array line_buffer

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
            offsetOfThePixel = (numberOfValuesInEachRow * senterY + senterX);
            countIncluded=(endx-startx+1)*(endy-starty+1);

            imageOut->data[offsetOfThePixel].red = sum_red/countIncluded;
            imageOut->data[offsetOfThePixel].green = sum_green/countIncluded;
            imageOut->data[offsetOfThePixel].blue = sum_blue/countIncluded;
        }

    }
    // free memory
    free(line_buffer);  
#pragma omp barrier
}

// Perform the final step, and save it as a ppm in imageOut
void performNewIdeaFinalization(AccurateImage *imageInSmall, AccurateImage *imageInLarge, PPMImage *imageOut) {

    imageOut->x = imageInSmall->x;
    imageOut->y = imageInSmall->y;

    // CHANGE: redid the finalization to use intcast instead of floors and if-tests.
    // CHANGE: used pragma omp for here.
#pragma omp for schedule(static)
    for(int i = 0; i < imageInSmall->x * imageInSmall->y; i++) {
        float value = (imageInLarge->data[i].red - imageInSmall->data[i].red);
        int val = (int)value;
        imageOut->data[i].red = (val < 255)?val:255;

        value = (imageInLarge->data[i].green - imageInSmall->data[i].green);
        val = (int)value;
        imageOut->data[i].green = (val < 255)?val:255;

        value = (imageInLarge->data[i].blue - imageInSmall->data[i].blue);
        val = (int)value;
        imageOut->data[i].blue = (val < 255)?val:255;
    }

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

    AccurateImage *imageUnchanged = convertImageToNewFormat(image); // save the unchanged image from input image
    AccurateImage *imageBuffer = createEmptyImage(image);
    AccurateImage *imageSmall = createEmptyImage(image);
    AccurateImage *imageBig = createEmptyImage(image);

    // CHANGE: added pragma omp parallel for everything below except write operations.

    // Process the tiny case:
#pragma omp parallel 
    {
        performNewIdeaIteration(imageSmall, imageUnchanged, 2);
        performNewIdeaIteration(imageBuffer, imageSmall, 2);
        performNewIdeaIteration(imageSmall, imageBuffer, 2);
        performNewIdeaIteration(imageBuffer, imageSmall, 2);
        performNewIdeaIteration(imageSmall, imageBuffer, 2);

        // Process the small case:
        performNewIdeaIteration(imageBig, imageUnchanged,3);
        performNewIdeaIteration(imageBuffer, imageBig,3);
        performNewIdeaIteration(imageBig, imageBuffer,3);
        performNewIdeaIteration(imageBuffer, imageBig,3);
        performNewIdeaIteration(imageBig, imageBuffer,3);

        // save tiny case result
        performNewIdeaFinalization(imageSmall,  imageBig, imageOut);
    }

    if(argc > 1) {
        writePPM("flower_tiny.ppm", imageOut);
    } else {
        writeStreamPPM(stdout, imageOut);
    }

#pragma omp parallel
    {
        // Process the medium case:
        performNewIdeaIteration(imageSmall, imageUnchanged, 5);
        performNewIdeaIteration(imageBuffer, imageSmall, 5);
        performNewIdeaIteration(imageSmall, imageBuffer, 5);
        performNewIdeaIteration(imageBuffer, imageSmall, 5);
        performNewIdeaIteration(imageSmall, imageBuffer, 5);

        // save small case
        performNewIdeaFinalization(imageBig,  imageSmall,imageOut);
    }

    if(argc > 1) {
        writePPM("flower_small.ppm", imageOut);
    } else {
        writeStreamPPM(stdout, imageOut);
    }

#pragma omp parallel
    {
        // process the large case
        performNewIdeaIteration(imageBig, imageUnchanged, 8);
        performNewIdeaIteration(imageBuffer, imageBig, 8);
        performNewIdeaIteration(imageBig, imageBuffer, 8);
        performNewIdeaIteration(imageBuffer, imageBig, 8);
        performNewIdeaIteration(imageBig, imageBuffer, 8);

        // save the medium case
        performNewIdeaFinalization(imageSmall,  imageBig, imageOut);
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

