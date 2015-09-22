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

void performNewIdeaIteration(AccurateImage *imgN, AccurateImage *imageOut, AccurateImage *imageIn, int size) {

  int L = size + size + 1;

  int W = imageIn->x;
  int H = imageIn->y;

  double iarr = 1.0 / L;
  // Horizontal
  for( int y_pos=0; y_pos<H; y_pos++) {
    double red_val = 0.0;
    double green_val = 0.0;
    double blue_val = 0.0;
    int ti = y_pos*W;
    int li = ti;
    int ri = ti+size;

    for(int j=0; j<size; j++) {
      red_val += imageIn->data[ti+j].red;
      green_val += imageIn->data[ti+j].green;
      blue_val += imageIn->data[ti+j].blue;
    }
    for(int j=0  ; j<=size ; j++) {
      red_val += imageIn->data[ri].red;
      green_val += imageIn->data[ri].green;
      blue_val += imageIn->data[ri].blue;
      imageOut->data[ti].red = red_val;// / (size+1+j);
      imageOut->data[ti].green = green_val;// / (size+1+j);
      imageOut->data[ti].blue = blue_val;// / (size+1+j);
      ti++; ri++;
    }
    for(int j=size+1; j<W-size; j++) {
      red_val += imageIn->data[ri].red - imageIn->data[li].red;
      green_val += imageIn->data[ri].green - imageIn->data[li].green;
      blue_val += imageIn->data[ri].blue - imageIn->data[li].blue;
      imageOut->data[ti].red = red_val;// * iarr;
      imageOut->data[ti].green = green_val;// * iarr;
      imageOut->data[ti].blue = blue_val;// * iarr;
      ti++; ri++; li++;
    }
    for(int j=1; j<=size; j++) {
      red_val -= imageIn->data[li].red;
      green_val -= imageIn->data[li].green;
      blue_val -= imageIn->data[li].blue;
      imageOut->data[ti].red = red_val;///(L-j);
      imageOut->data[ti].green = green_val;///(L-j);
      imageOut->data[ti].blue = blue_val;///(L-j);
      ti++; li++;
    }
  }
  // Vertical
  for(int i=0; i<W; i++) {
    double red_val = 0.0;
    double green_val = 0.0;
    double blue_val = 0.0;
    int ti = i;
    int li = ti;
    int ri = ti+size*W;

    double x = 1.0 / L;
    if (i<size) {
      x = 1.0/ (size+1+i);
    } else if (i > (W-size-1)) {
      x = 1.0 / (size + (W - i));
    }

    for(int j=0; j<size; j++) {
      red_val += imageOut->data[ti+j*W].red;
      green_val += imageOut->data[ti+j*W].green;
      blue_val += imageOut->data[ti+j*W].blue;
    }
    for(int j=0  ; j<=size ; j++) {
      red_val += imageOut->data[ri].red;
      green_val += imageOut->data[ri].green;
      blue_val += imageOut->data[ri].blue;
      imgN->data[ti].red = red_val / (size+1+j) * x;
      imgN->data[ti].green = green_val / (size+1+j) * x;
      imgN->data[ti].blue = blue_val / (size+1+j) * x;
      ri+=W; ti+=W;
    }
    for(int j=size+1; j<H-size; j++) {
      red_val += imageOut->data[ri].red - imageOut->data[li].red;
      green_val += imageOut->data[ri].green - imageOut->data[li].green;
      blue_val += imageOut->data[ri].blue - imageOut->data[li].blue;
      imgN->data[ti].red = red_val * iarr * x;
      imgN->data[ti].green = green_val * iarr * x;
      imgN->data[ti].blue = blue_val * iarr * x;
      li+=W; ri+=W; ti+=W;
    }
    for(int j=1; j<=size  ; j++) {
      red_val -= imageOut->data[li].red;
      green_val -= imageOut->data[li].green;
      blue_val -= imageOut->data[li].blue;
      imgN->data[ti].red = red_val / (L-j) * x;
      imgN->data[ti].green = green_val / (L-j) * x;
      imgN->data[ti].blue = blue_val / (L-j) * x;
      li+=W; ti+=W;
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
    } else   {
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
