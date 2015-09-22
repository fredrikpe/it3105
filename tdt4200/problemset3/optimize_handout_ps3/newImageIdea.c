#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ppm.h"

// Image from:
// http://7-themes.com/6971875-funny-flowers-pictures.html

typedef struct {
     double red,green,blue;
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
		imageAccurate->data[i].red   = (double) image->data[i].red;
		imageAccurate->data[i].green = (double) image->data[i].green;
		imageAccurate->data[i].blue  = (double) image->data[i].blue;
	}
	imageAccurate->x = image->x;
	imageAccurate->y = image->y;

	return imageAccurate;
}

void performNewIdeaIteration(AccurateImage *imageOut, AccurateImage *imageTmp, AccurateImage *imageIn, int size) {

  int L = size + size + 1;
  int W = imageIn->x;
  int H = imageIn->y;
  double rec = 1.0 / L;

  // Horizontal
  for( int y_pos=0; y_pos<H; y_pos++)
  {
    double red_val = 0.0;
    double green_val = 0.0;
    double blue_val = 0.0;
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
      imageTmp->data[cur].red = red_val;
      imageTmp->data[cur].green = green_val;
      imageTmp->data[cur].blue = blue_val;
      cur++; next++;
    }
    for(int j=size+1; j<W-size; j++) // middle part
    {
      red_val += imageIn->data[next].red - imageIn->data[prev].red;
      green_val += imageIn->data[next].green - imageIn->data[prev].green;
      blue_val += imageIn->data[next].blue - imageIn->data[prev].blue;
      imageTmp->data[cur].red = red_val;
      imageTmp->data[cur].green = green_val;
      imageTmp->data[cur].blue = blue_val;
      cur++; next++; prev++;
    }
    for(int j=1; j<=size; j++) // end edge
    {
      red_val -= imageIn->data[prev].red;
      green_val -= imageIn->data[prev].green;
      blue_val -= imageIn->data[prev].blue;
      imageTmp->data[cur].red = red_val;
      imageTmp->data[cur].green = green_val;
      imageTmp->data[cur].blue = blue_val;
      cur++; prev++;
    }
  }
  // Vertical
  for(int i=0; i<W; i++)
  {
    double red_val = 0.0;
    double green_val = 0.0;
    double blue_val = 0.0;
    int cur = i;
    int prev = cur;
    int next = cur + size*W;

    double x = 1.0 / L;
    if (i<size) {
      x = 1.0/ (size+1+i);
    } else if (i > (W-size-1)) {
      x = 1.0 / (size + (W - i));
    }

    for(int j=0; j<size; j++)
    {
      red_val += imageTmp->data[cur+j*W].red;
      green_val += imageTmp->data[cur+j*W].green;
      blue_val += imageTmp->data[cur+j*W].blue;
    }
    for(int j=0  ; j<=size ; j++) // Start edge
    {
      double y = 1.0 / (size + 1 + j);
      red_val += imageTmp->data[next].red;
      green_val += imageTmp->data[next].green;
      blue_val += imageTmp->data[next].blue;
      imageOut->data[cur].red = red_val * y * x;
      imageOut->data[cur].green = green_val * y * x;
      imageOut->data[cur].blue = blue_val * y * x;
      next+=W; cur+=W;
    }
    for(int j=size+1; j<H-size; j++) // Middle part
    {
      red_val += imageTmp->data[next].red - imageTmp->data[prev].red;
      green_val += imageTmp->data[next].green - imageTmp->data[prev].green;
      blue_val += imageTmp->data[next].blue - imageTmp->data[prev].blue;
      imageOut->data[cur].red = red_val * rec * x;
      imageOut->data[cur].green = green_val * rec * x;
      imageOut->data[cur].blue = blue_val * rec * x;
      prev+=W; next+=W; cur+=W;
    }
    for(int j=1; j<=size  ; j++) // End edge
    {
      double y = 1.0 / (L - j);
      red_val -= imageTmp->data[prev].red;
      green_val -= imageTmp->data[prev].green;
      blue_val -= imageTmp->data[prev].blue;
      imageOut->data[cur].red = red_val * y * x;
      imageOut->data[cur].green = green_val * y * x;
      imageOut->data[cur].blue = blue_val * y * x;
      prev+=W; cur+=W;
    }
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
    double value = (imageInLarge->data[i].red - imageInSmall->data[i].red);
    if(value > 255)
      imageOut->data[i].red = 255;
    else if (value < -1.0) {
      value = 257.0+value;
      if(value > 255)
        imageOut->data[i].red = 255;
      else
        imageOut->data[i].red = floor(value);
    } else if (value > -1.0 && value < 0.0) {
      imageOut->data[i].red = 0;
    } else {
      imageOut->data[i].red = floor(value);
    }

    value = (imageInLarge->data[i].green - imageInSmall->data[i].green);
    if(value > 255)
      imageOut->data[i].green = 255;
    else if (value < -1.0) {
      value = 257.0+value;
      if(value > 255)
        imageOut->data[i].green = 255;
      else
        imageOut->data[i].green = floor(value);
    } else if (value > -1.0 && value < 0.0) {
      imageOut->data[i].green = 0;
    } else {
      imageOut->data[i].green = floor(value);
    }


    value = (imageInLarge->data[i].blue - imageInSmall->data[i].blue);
    if(value > 255)
      imageOut->data[i].blue = 255;
    else if (value < -1.0) {
      value = 257.0+value;
      if(value > 255)
        imageOut->data[i].blue = 255;
      else
        imageOut->data[i].blue = floor(value);
    } else if (value > -1.0 && value < 0.0) {
      imageOut->data[i].blue = 0;
    } else   {
      imageOut->data[i].blue = floor(value);
    }
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
  AccurateImage *it3 = convertImageToNewFormat(image);
	// Process the tiny case:
	int size = 2;
	performNewIdeaIteration(it2, it3, it1, size);
	performNewIdeaIteration(it1, it3, it2, size);
	performNewIdeaIteration(it2, it3, it1, size);
	performNewIdeaIteration(it1, it3, it2, size);
  performNewIdeaIteration(it2, it3, it1, size);



  AccurateImage *is1 = convertImageToNewFormat(image);
  AccurateImage *is2 = convertImageToNewFormat(image);
  AccurateImage *is3 = convertImageToNewFormat(image);
  // Process the small case:
  size = 3;
  performNewIdeaIteration(is2, is3, is1, size);
  performNewIdeaIteration(is1, is3, is2, size);
  performNewIdeaIteration(is2, is3, is1, size);
  performNewIdeaIteration(is1, is3, is2, size);
  performNewIdeaIteration(is2, is3, is1, size);

  AccurateImage *im1 = convertImageToNewFormat(image);
  AccurateImage *im2 = convertImageToNewFormat(image);
  AccurateImage *im3 = convertImageToNewFormat(image);
  // Process the medium case:
  size = 5;
  performNewIdeaIteration(im2, im3, im1, size);
  performNewIdeaIteration(im1, im3, im2, size);
  performNewIdeaIteration(im2, im3, im1, size);
  performNewIdeaIteration(im1, im3, im2, size);
  performNewIdeaIteration(im2, im3, im1, size);


  AccurateImage *il1 = convertImageToNewFormat(image);
  AccurateImage *il2 = convertImageToNewFormat(image);
  AccurateImage *il3 = convertImageToNewFormat(image);
  // Process the large case:
  size = 8;
  performNewIdeaIteration(il2, il3, il1, size);
  performNewIdeaIteration(il1, il3, il2, size);
  performNewIdeaIteration(il2, il3, il1, size);
  performNewIdeaIteration(il1, il3, il2, size);
  performNewIdeaIteration(il2, il3, il1, size);

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
