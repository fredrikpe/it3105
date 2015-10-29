#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ppm.h"


// Image from:
// http://7-themes.com/6971875-funny-flowers-pictures.html

/*typedef struct {
    float red,green,blue; // TODO: this must probably be changed for a SIMD version.
} AccuratePixel;*/
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
	imageAccurate->data = (AccuratePixel*)malloc(image->x * image->y * sizeof(AccuratePixel));
	for(int i = 0; i < image->x * image->y; i++) {
    v4f rgb = {(float) image->data[i].red, (float) image->data[i].green, (float) image->data[i].blue, 0};
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

void horizontalAvg(AccurateImage *imageOut, AccuratePixel *line_buffer, float rec, int W, int starty, int endy, int senterY, int size)
{
  v4f sum_rgb = {0, 0, 0, 0};
  for(int i=0; i<W; i++)
  {
    int startx = i-size;
    int endx = i+size;
    if (startx <=0){
      startx = 0;
      if(i==0){
        for (int x=startx; x < endx; x++){
          sum_rgb += line_buffer[x].rgb;
        }
      }
      sum_rgb +=line_buffer[endx].rgb;
      rec = 1.0 / ((endx+1)*(endy-starty+1));
    }else if (endx >= W){
      endx = W-1;
      sum_rgb -= line_buffer[startx-1].rgb;
      rec = 1.0 / ((endx-startx+1)*(endy-starty+1));
    }else{
      sum_rgb += (line_buffer[endx].rgb-line_buffer[startx-1].rgb);
    }
    // we save each pixel in the output image
    imageOut->data[W * senterY + i].rgb = sum_rgb * rec;
	}
}

// Perform the new idea:
void performNewIdeaIteration(AccurateImage *imageOut, AccurateImage *imageIn, int size)
{
	float rec = 1.0 / (size * 2 + 1);

  int W = imageIn->x;
  int H = imageIn->y;

	// line buffer that will save the sum of some pixel in the column
	AccuratePixel *line_buffer = (AccuratePixel*)malloc(W*sizeof(AccuratePixel));
	memset(line_buffer,0,W*sizeof(AccuratePixel));

  int starty;
  int endy;

	// Iterate over each line of pixelx.
	for(int senterY = 0; senterY <= size; senterY++) {
		// first and last line considered  by the computation of the pixel in the line senterY
		starty = 0;
		endy = senterY+size;
		starty = 0;
		if(senterY == 0)
    {
			for(int line_y=starty; line_y < endy; line_y++){
				for(int i=0; i<W; i++){
					line_buffer[i].rgb += imageIn->data[W*line_y+i].rgb;
				}
			}
		}
    for(int i=0; i<W; i++)
    {
      line_buffer[i].rgb += imageIn->data[W*endy+i].rgb;
    }
    horizontalAvg(imageOut, line_buffer, rec, W, starty, endy, senterY, size);
  }
  for(int senterY = size+1; senterY < H-size; senterY++) {
    starty = senterY-size;
    endy = senterY+size;
    for(int i=0; i<W; i++)
    {
      line_buffer[i].rgb+=imageIn->data[W*endy+i].rgb-imageIn->data[W*(starty-1)+i].rgb;
    }
    horizontalAvg(imageOut, line_buffer, rec, W, starty, endy, senterY, size);
  }
  endy = H-1;
  for(int senterY = H-size; senterY < H; senterY++) {
    starty = senterY-size;
    for(int i=0; i<W; i++)
    {
      line_buffer[i].rgb -= imageIn->data[W*(starty-1)+i].rgb;
    }
    horizontalAvg(imageOut, line_buffer, rec, W, starty, endy, senterY, size);
  }
  free(line_buffer);
}


int getNewValue(float old_value)
{
  //if (old_value < -1)
  //  old_value += 257;
  //if(old_value > 255)
  //  return 255;
  return old_value;
}
// Perform the final step, and save it as a ppm in imageOut
void performNewIdeaFinalization(AccurateImage *imageInSmall, AccurateImage *imageInLarge, PPMImage *imageOut)
{
	imageOut->x = imageInSmall->x;
	imageOut->y = imageInSmall->y;
	for(int i = 0; i < imageInSmall->x * imageInSmall->y; i++)
  {
    v4f values = imageInLarge->data[i].rgb - imageInSmall->data[i].rgb;
		imageOut->data[i].red = getNewValue(values[0]);
		imageOut->data[i].green = getNewValue(values[1]);
		imageOut->data[i].blue = getNewValue(values[2]);
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
	performNewIdeaFinalization(imageSmall, imageBig, imageOut);
	if(argc > 1) {
		writePPM("flower_tiny.ppm", imageOut);
	} else {
		writeStreamPPM(stdout, imageOut);
	}

	// Process the medium case:
  process(imageSmall, imageUnchanged, imageBuffer, 5);

	// save small case
	performNewIdeaFinalization(imageBig, imageSmall,imageOut);
	if(argc > 1) {
		writePPM("flower_small.ppm", imageOut);
	} else {
		writeStreamPPM(stdout, imageOut);
	}

	// process the large case
  process(imageBig, imageUnchanged, imageBuffer, 8);

  // save the medium case
	performNewIdeaFinalization(imageSmall, imageBig, imageOut);
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
