#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

void performNewIdeaIteration(AccurateImage *imageOut, AccurateImage *imageIn, int size) {

  int L = size + size + 1;
  int W = imageIn->x;
  int H = imageIn->y;
  float rec = 1.0 / L;

  // Horizontal
  for( int y_pos=0; y_pos<H; y_pos++)
  {
    float red_val = 0.0;
    float green_val = 0.0;
    float blue_val = 0.0;
    int cur = y_pos*W;
    int prev = cur;
    int next = cur+size;

    for(int j=0; j<size; j++) // initilize values
    {
      red_val += imageIn->data[cur+j].red;
      green_val += imageIn->data[cur+j].green;
      blue_val += imageIn->data[cur+j].blue;
    }
    for(int j=0  ; j<=size ; j++) // start edge
    {
      red_val += imageIn->data[next].red;
      green_val += imageIn->data[next].green;
      blue_val += imageIn->data[next].blue;
      imageOut->data[cur].red = red_val;
      imageOut->data[cur].green = green_val;
      imageOut->data[cur].blue = blue_val;
      cur++; next++;
    }
    for(int j=size+1; j<W-size; j++) // middle part
    {
      red_val += imageIn->data[next].red - imageIn->data[prev].red;
      green_val += imageIn->data[next].green - imageIn->data[prev].green;
      blue_val += imageIn->data[next].blue - imageIn->data[prev].blue;
      imageOut->data[cur].red = red_val;
      imageOut->data[cur].green = green_val;
      imageOut->data[cur].blue = blue_val;
      cur++; next++; prev++;
    }
    for(int j=1; j<=size; j++) // end edge
    {
      red_val -= imageIn->data[prev].red;
      green_val -= imageIn->data[prev].green;
      blue_val -= imageIn->data[prev].blue;
      imageOut->data[cur].red = red_val;
      imageOut->data[cur].green = green_val;
      imageOut->data[cur].blue = blue_val;
      cur++; prev++;
    }
  }
  // Vertical
  for(int i=0; i<W; i++)
  {
    float red_val = 0.0;
    float green_val = 0.0;
    float blue_val = 0.0;
    int cur = i;
    int prev = cur;
    int next = cur + size*W;

    float x = 1.0 / L;
    if (i<size) {
      x = 1.0/ (size+1+i);
    } else if (i > (W-size-1)) {
      x = 1.0 / (size + (W - i));
    }

    for(int j=0; j<size; j++)
    {
      red_val += imageOut->data[cur+j*W].red;
      green_val += imageOut->data[cur+j*W].green;
      blue_val += imageOut->data[cur+j*W].blue;
    }
    for(int j=0  ; j<=size ; j++) // Start edge
    {
      float y = 1.0 / (size + 1 + j);
      red_val += imageOut->data[next].red;
      green_val += imageOut->data[next].green;
      blue_val += imageOut->data[next].blue;
      imageIn->data[cur].red = red_val * y * x;
      imageIn->data[cur].green = green_val * y * x;
      imageIn->data[cur].blue = blue_val * y * x;
      next+=W; cur+=W;
    }
    for(int j=size+1; j<H-size; j++) // Middle part
    {
      red_val += imageOut->data[next].red - imageOut->data[prev].red;
      green_val += imageOut->data[next].green - imageOut->data[prev].green;
      blue_val += imageOut->data[next].blue - imageOut->data[prev].blue;
      imageIn->data[cur].red = red_val * rec * x;
      imageIn->data[cur].green = green_val * rec * x;
      imageIn->data[cur].blue = blue_val * rec * x;
      prev+=W; next+=W; cur+=W;
    }
    for(int j=1; j<=size  ; j++) // End edge
    {
      float y = 1.0 / (L - j);
      red_val -= imageOut->data[prev].red;
      green_val -= imageOut->data[prev].green;
      blue_val -= imageOut->data[prev].blue;
      imageIn->data[cur].red = red_val * y * x;
      imageIn->data[cur].green = green_val * y * x;
      imageIn->data[cur].blue = blue_val * y * x;
      prev+=W; cur+=W;
    }
  }
}

float getNewValue(float old_value) {
  old_value = floor(old_value);
  if(old_value > 255)
    return 255;
  else if (old_value < -1.0) {   //old_value > -1.0 &&
    old_value = 257.0+old_value;
    if(old_value > 255)
      return 255;
    else
      return  old_value;
  } else if (old_value == -1.0) {
    return 0;
  } else {
    return old_value;
  }
}

// Perform the final step, and return it as ppm.
PPMImage * performNewIdeaFinalization(AccurateImage *imageInSmall, AccurateImage *imageInLarge) {
  PPMImage *imageOut;
  imageOut = (PPMImage *)malloc(sizeof(PPMImage));
  imageOut->data = (PPMPixel*)malloc(imageInSmall->x * imageInSmall->y * sizeof(PPMPixel));

  imageOut->x = imageInSmall->x;
  imageOut->y = imageInSmall->y;

  for(int i = 0; i < imageInSmall->x * imageInSmall->y; i++) {
    float value = (imageInLarge->data[i].red - imageInSmall->data[i].red);
    imageOut->data[i].red = getNewValue(value);
    value = (imageInLarge->data[i].green - imageInSmall->data[i].green);
    imageOut->data[i].green = getNewValue(value);
    value = (imageInLarge->data[i].blue - imageInSmall->data[i].blue);
    imageOut->data[i].blue = getNewValue(value);
  }
  return imageOut;
}



int main(int argc, char** argv) {

	PPMImage *image;

	if(argc > 1) {
		image = readPPM("flower.ppm");
	} else {
		image = readStreamPPM(stdin);
	}
	AccurateImage *it1 = convertImageToNewFormat(image);
	AccurateImage *it2 = convertImageToNewFormat(image);
	// Process the tiny case:
	int size = 2;
	performNewIdeaIteration(it1, it2, size);
	performNewIdeaIteration(it1, it2, size);
	performNewIdeaIteration(it1, it2, size);
	performNewIdeaIteration(it1, it2, size);
  performNewIdeaIteration(it1, it2, size);



  AccurateImage *is1 = convertImageToNewFormat(image);
  AccurateImage *is2 = convertImageToNewFormat(image);
  // Process the small case:
  size = 3;
  performNewIdeaIteration(is1, is2, size);
  performNewIdeaIteration(is1, is2, size);
  performNewIdeaIteration(is1, is2, size);
  performNewIdeaIteration(is1, is2, size);
  performNewIdeaIteration(is1, is2, size);

  AccurateImage *im1 = convertImageToNewFormat(image);
  AccurateImage *im2 = convertImageToNewFormat(image);
  // Process the medium case:
  size = 5;
  performNewIdeaIteration(im1, im2, size);
  performNewIdeaIteration(im1, im2, size);
  performNewIdeaIteration(im1, im2, size);
  performNewIdeaIteration(im1, im2, size);
  performNewIdeaIteration(im1, im2, size);


  AccurateImage *il1 = convertImageToNewFormat(image);
  AccurateImage *il2 = convertImageToNewFormat(image);
  // Process the large case:
  size = 8;
  performNewIdeaIteration(il1, il2, size);
  performNewIdeaIteration(il1, il2, size);
  performNewIdeaIteration(il1, il2, size);
  performNewIdeaIteration(il1, il2, size);
  performNewIdeaIteration(il1, il2, size);

	// Save the images.
	PPMImage *final_tiny = performNewIdeaFinalization(it2, is2);
	PPMImage *final_small = performNewIdeaFinalization(is2, im2);
	PPMImage *final_medium = performNewIdeaFinalization(im2, il2);

	if(argc > 1) {
		writePPM("flower_tiny.ppm", final_tiny);
		writePPM("flower_small.ppm", final_small);
		writePPM("flower_medium.ppm", final_medium);
	} else {
		writeStreamPPM(stdout, final_tiny);
		writeStreamPPM(stdout, final_small);
		writeStreamPPM(stdout, final_medium);
	}
	return 0;
}
