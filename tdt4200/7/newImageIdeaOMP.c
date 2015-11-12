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

AccurateImage *createEmptyImage(PPMImage *image)
{
  AccurateImage *imageAccurate;
  imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));
  imageAccurate->data = (AccuratePixel*)malloc(image->x * image->y * sizeof(AccuratePixel));
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


void performNewIdeaIteration(AccurateImage *imageOut, AccurateImage *imageIn,int size)
{
  int W = imageIn->x;
  int H = imageIn->y;
  int each = H/4;

  #pragma omp parallel num_threads(4)
  {
    float sum_red = 0;
    float sum_blue = 0;
    float sum_green =0;
    float rec = 1.0 / (2*size + 1);

    // line buffer that will save the sum of some pixel in the column
    AccuratePixel *line_buffer = (AccuratePixel*) malloc(W*sizeof(AccuratePixel));
    memset(line_buffer,0,W*sizeof(AccuratePixel));

    #pragma omp for
    for(int senterY = 0; senterY < H; senterY++) {
      if (senterY == 0) {
        for(int y=0; y < size; y++) {
          for(int i=0; i<W; i++){
            line_buffer[i].blue+=imageIn->data[W*y+i].blue;
            line_buffer[i].red+=imageIn->data[W*y+i].red;
            line_buffer[i].green+=imageIn->data[W*y+i].green;
          }
        }
      }
      if (senterY == each || senterY == 2*each || senterY == 3*each) {
        for(int y=senterY-size-1; y < senterY+size; y++) {
          for(int i=0; i<W; i++){
            line_buffer[i].blue+=imageIn->data[W*y+i].blue;
            line_buffer[i].red+=imageIn->data[W*y+i].red;
            line_buffer[i].green+=imageIn->data[W*y+i].green;
          }
        }
      }
      int starty = senterY-size;
      int endy = senterY+size;

      if (starty <=0) {
        starty = 0;
        for(int i=0; i<W; i++){
          // add the next pixel of the next line in the column x
          line_buffer[i].blue+=imageIn->data[W*endy+i].blue;
          line_buffer[i].red+=imageIn->data[W*endy+i].red;
          line_buffer[i].green+=imageIn->data[W*endy+i].green;
        }
      }
      else if (endy >= H ){
        // for the last lines, we just need to subtract the first added line
        endy = H-1;
        for(int i=0; i<W; i++){
          line_buffer[i].blue-=imageIn->data[W*(starty-1)+i].blue;
          line_buffer[i].red-=imageIn->data[W*(starty-1)+i].red;
          line_buffer[i].green-=imageIn->data[W*(starty-1)+i].green;
        }
      }else{
        // general case
        // add the next line and remove the first added line
        for(int i=0; i<W; i++){
          line_buffer[i].blue+=imageIn->data[W*endy+i].blue-imageIn->data[W*(starty-1)+i].blue;
          line_buffer[i].red+=imageIn->data[W*endy+i].red-imageIn->data[W*(starty-1)+i].red;
          line_buffer[i].green+=imageIn->data[W*endy+i].green-imageIn->data[W*(starty-1)+i].green;
        }
      }
      sum_green = 0;
      sum_red = 0;
      sum_blue = 0;
      for(int senterX = 0; senterX < W; senterX++) {
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
          rec = 1.0/((endx+1)*(endy-starty+1));
        }else if (endx >= W){
          endx = W-1;
          sum_red -=line_buffer[startx-1].red;
          sum_green -=line_buffer[startx-1].green;
          sum_blue -=line_buffer[startx-1].blue;
          rec = 1.0/ ((W-startx)*(endy-starty+1));
        }else{
          sum_red += (line_buffer[endx].red-line_buffer[startx-1].red);
          sum_green += (line_buffer[endx].green-line_buffer[startx-1].green);
          sum_blue += (line_buffer[endx].blue-line_buffer[startx-1].blue);
        }
        imageOut->data[senterY*W + senterX].red = sum_red * rec;
        imageOut->data[senterY*W + senterX].green = sum_green * rec;
        imageOut->data[senterY*W + senterX].blue = sum_blue * rec;
      }
    }
    free(line_buffer);
  }
}

    // Perform the final step, and save it as a ppm in imageOut
void performNewIdeaFinalization(AccurateImage *imageInSmall, AccurateImage *imageInLarge, PPMImage *imageOut)
{
  imageOut->x = imageInSmall->x;
  imageOut->y = imageInSmall->y;

  for(int i=0; i<imageInSmall->x * imageInSmall->y; i+=1)
  {
    float value = imageInLarge->data[i].red - imageInSmall->data[i].red;
    imageOut->data[i].red = (int)value;
    value = imageInLarge->data[i].green - imageInSmall->data[i].green;
    imageOut->data[i].green = (int)value;
    value = imageInLarge->data[i].blue - imageInSmall->data[i].blue;
    imageOut->data[i].blue = (int)value;
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
