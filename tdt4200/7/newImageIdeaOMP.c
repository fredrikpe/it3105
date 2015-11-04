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

// Perform the new idea:
// The code in this function should run in parallel
// Try to find a good strategy for dividing the problem into individual parts.
// Using OpenMP inside this function itself might be avoided
// You may be able to do this only with a single OpenMP directive
void performNewIdeaIteration(AccurateImage *imageOut, AccurateImage *imageIn,int size) {
	int countIncluded = 0;
	int offsetOfThePixel=0;
	float sum_red = 0;
	float sum_blue = 0;
	float sum_green =0;
	int W = imageIn->x;
  int H = imageIn->y;

  AccurateImage *image_buffer = (AccurateImage*)malloc(sizeof(AccurateImage));
  image_buffer->data = (AccuratePixel*)malloc(W*H*sizeof(AccuratePixel));
  memset(image_buffer->data, 0, W*H*sizeof(AccuratePixel));

  for(int y=0; y < size; y++){
    for(int x=0; x<W; x++){
      image_buffer->data[x].blue+=imageIn->data[y*W + x].blue;
      image_buffer->data[x].red+=imageIn->data[y*W + x].red;
      image_buffer->data[x].green+=imageIn->data[y*W + x].green;
    }
  }
  #pragma omp parallel for schedule(static, 1)
	for(int senterY = 0; senterY < imageIn->y; senterY++) {
		// first and last line considered  by the computation of the pixel in the line senterY
		int starty = senterY-size;
		int endy = senterY+size;

		// Initialize and update the line_buffer.
		// For OpenMP this might cause problems
		// Separating out the initialization part might help
		if (starty <=0){
			starty = 0;
			for(int i=0; i<imageIn->x; i++){
				// add the next pixel of the next line in the column x
				image_buffer->data[W*senterY + i].blue+=imageIn->data[W*endy+i].blue;
				image_buffer->data[W*senterY + i].red+=imageIn->data[W*endy+i].red;
				image_buffer->data[W*senterY + i].green+=imageIn->data[W*endy+i].green;
			}

		}

		else if (endy >= imageIn->y ){
			// for the last lines, we just need to subtract the first added line
			endy = imageIn->y-1;
			for(int i=0; i<imageIn->x; i++){
				image_buffer->data[W*senterY + i].blue-=imageIn->data[W*(starty-1)+i].blue;
				image_buffer->data[W*senterY + i].red-=imageIn->data[W*(starty-1)+i].red;
				image_buffer->data[W*senterY + i].green-=imageIn->data[W*(starty-1)+i].green;
			}
		}else{
			// general case
			// add the next line and remove the first added line
			for(int i=0; i<imageIn->x; i++){
				image_buffer->data[W*senterY + i].blue+=imageIn->data[W*endy+i].blue-imageIn->data[W*(starty-1)+i].blue;
				image_buffer->data[W*senterY + i].red+=imageIn->data[W*endy+i].red-imageIn->data[W*(starty-1)+i].red;
				image_buffer->data[W*senterY + i].green+=imageIn->data[W*endy+i].green-imageIn->data[W*(starty-1)+i].green;
			}
		}
		// End of line_buffer initialisation.


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
						sum_red += image_buffer->data[senterY*W + x].red;
						sum_green += image_buffer->data[senterY*W + x].green;
						sum_blue += image_buffer->data[senterY*W + x].blue;
					}
				}
				sum_red +=image_buffer->data[senterY*W + endx].red;
				sum_green +=image_buffer->data[senterY*W + endx].green;
				sum_blue +=image_buffer->data[senterY*W + endx].blue;
			}else if (endx >= imageIn->x){
				endx = imageIn->x-1;
				sum_red -=image_buffer->data[senterY*W + startx-1].red;
				sum_green -=image_buffer->data[senterY*W + startx-1].green;
				sum_blue -=image_buffer->data[senterY*W + startx-1].blue;

			}else{
				sum_red += (image_buffer->data[senterY*W + endx].red-image_buffer->data[senterY*W + startx-1].red);
				sum_green += (image_buffer->data[senterY*W + endx].green-image_buffer->data[senterY*W + startx-1].green);
				sum_blue += (image_buffer->data[senterY*W + endx].blue-image_buffer->data[senterY*W + startx-1].blue);
			}

			// we save each pixel in the output image
			offsetOfThePixel = (W * senterY + senterX);
			countIncluded=(endx-startx+1)*(endy-starty+1);

			imageOut->data[offsetOfThePixel].red = sum_red/countIncluded;
			imageOut->data[offsetOfThePixel].green = sum_green/countIncluded;
			imageOut->data[offsetOfThePixel].blue = sum_blue/countIncluded;
		}

	}

	// free memory
  free(image_buffer);
  free(image_buffer->data);
}

int getValue(float value)
{
  if(value > 255) {
    return 255;
  } else if (value < -1.0) {
    value += 257;
    if(value > 255)
      return 255;
  } else if (value > -1.0 && value < 0.0) {
    return 0;
  }
  return (int) value;
}

    // Perform the final step, and save it as a ppm in imageOut
void performNewIdeaFinalization(AccurateImage *imageInSmall, AccurateImage *imageInLarge, PPMImage *imageOut)
{
  imageOut->x = imageInSmall->x;
  imageOut->y = imageInSmall->y;

  for(int i=0; i<imageInSmall->x * imageInSmall->y; i+=1)
  {
    float value = imageInLarge->data[i].red - imageInSmall->data[i].red;
    imageOut->data[i].red = getValue(value);
    value = imageInLarge->data[i].green - imageInSmall->data[i].green;
    imageOut->data[i].green = getValue(value);
    value = imageInLarge->data[i].blue - imageInSmall->data[i].blue;
    imageOut->data[i].blue = getValue(value);
  }
}

void fiveIterations(AccurateImage *imageNew, AccurateImage *imageUnchanged, AccurateImage *imageBuffer, int size)
{
  performNewIdeaIteration(imageNew, imageUnchanged, size);
  performNewIdeaIteration(imageBuffer, imageNew, size);
  performNewIdeaIteration(imageNew, imageBuffer, size);
  performNewIdeaIteration(imageBuffer, imageNew, size);
  performNewIdeaIteration(imageNew, imageBuffer, size);
}


int main(int argc, char** argv) {

	PPMImage *image;

	if(argc > 1) {
		image = readPPM("flower.ppm");
	} else {
		image = readStreamPPM(stdin);
	}

	AccurateImage *imageUnchanged = convertImageToNewFormat(image); // save the unchanged image from input image
	AccurateImage *imageBuffer = createEmptyImage(image);
	AccurateImage *imageSmall = createEmptyImage(image);
	AccurateImage *imageBig = createEmptyImage(image);

	PPMImage *imageOut;
	imageOut = (PPMImage *)malloc(sizeof(PPMImage));
	imageOut->data = (PPMPixel*)malloc(image->x * image->y * sizeof(PPMPixel));

	// Process the tiny case:
	fiveIterations(imageSmall, imageUnchanged, imageBuffer, 2);

	// Process the small case:
  fiveIterations(imageBig, imageUnchanged, imageBuffer, 3);

	// save tiny case result
	performNewIdeaFinalization(imageSmall,  imageBig, imageOut);
	if(argc > 1) {
		writePPM("flower_tiny.ppm", imageOut);
	} else {
		writeStreamPPM(stdout, imageOut);
	}

	// Process the medium case:
  fiveIterations(imageSmall, imageUnchanged, imageBuffer, 5);

	// save small case
	performNewIdeaFinalization(imageBig,  imageSmall,imageOut);
	if(argc > 1) {
		writePPM("flower_small.ppm", imageOut);
	} else {
		writeStreamPPM(stdout, imageOut);
	}

	// process the large case
  fiveIterations(imageBig, imageUnchanged, imageBuffer, 8);

	// save the medium case
	performNewIdeaFinalization(imageSmall,  imageBig, imageOut);
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
