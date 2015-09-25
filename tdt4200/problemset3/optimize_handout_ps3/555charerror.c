#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ppm.h"

// Image from:
// http://7-themes.com/6971875-funny-flowers-pictures.html

typedef struct {
     unsigned char red,green,blue;
} AccuratePixel;

typedef struct {
     int x, y;
     AccuratePixel *data;
} AccurateImage;

int W;
int H;

// Convert ppm to high precision format.
AccurateImage *convertImageToNewFormat(PPMImage *image) {
	// Make a copy

  W = image->x;
  H = image->y;

	AccurateImage *imageAccurate;
	imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));
	imageAccurate->data = (AccuratePixel*)malloc(W * H * sizeof(AccuratePixel));

	for(int i = 0; i < W * H; i++) {
    imageAccurate->data[i].red   = (unsigned char) image->data[i].red;
    imageAccurate->data[i].green = (unsigned char) image->data[i].green;
    imageAccurate->data[i].blue  = (unsigned char) image->data[i].blue;
	}
	imageAccurate->x = W;
	imageAccurate->y = H;

	return imageAccurate;
}

void performNewIdeaIteration(AccurateImage *imageOut, AccurateImage *imageIn, int size) {

  int L = size + size + 1;
  float rec = 1.0 / L;

  // Horizontal
  for( int y_pos=0; y_pos<H; y_pos++)
  {
    float red_val = 0.0;
    float green_val = 0.0;
    float blue_val = 0.0;
    int cur = y_pos;
    int prev = cur*W;
    int next = cur*W+size;

    for(int j=0; j<size; j++) // initilize values
    {
      red_val += imageIn->data[cur*W+j].red;
      green_val += imageIn->data[cur*W+j].green;
      blue_val += imageIn->data[cur*W+j].blue;
    }
    for(int j=0  ; j<=size ; j++) // start edge
    {
      red_val += imageIn->data[next].red;
      green_val += imageIn->data[next].green;
      blue_val += imageIn->data[next].blue;
      imageOut->data[cur].red = red_val;
      imageOut->data[cur].green = green_val;
      imageOut->data[cur].blue = blue_val;
      cur+=H; next++;
    }
    for(int j=size+1; j<W-size; j++) // middle part
    {
      red_val += imageIn->data[next].red - imageIn->data[prev].red;
      green_val += imageIn->data[next].green - imageIn->data[prev].green;
      blue_val += imageIn->data[next].blue - imageIn->data[prev].blue;

      imageOut->data[cur].red = red_val;
      imageOut->data[cur].green = green_val;
      imageOut->data[cur].blue = blue_val;
      cur+=H; next++; prev++;
    }
    for(int j=1; j<=size; j++) // end edge
    {
      red_val -= imageIn->data[prev].red;
      green_val -= imageIn->data[prev].green;
      blue_val -= imageIn->data[prev].blue;
      imageOut->data[cur].red = red_val;
      imageOut->data[cur].green = green_val;
      imageOut->data[cur].blue = blue_val;
      cur+=H; prev++;
    }
  }

  // Vertical
  for(int i=0; i<W; i++)
  {
    float red_val = 0.0;
    float green_val = 0.0;
    float blue_val = 0.0;
    int cur = i;
    int prev = cur*H;
    int next = cur*H + size;

    float x = 1.0 / L;
    //x = (i<size) ? 1.0/(size+1+i) : x;
    //x = (i>(W-size-1)) ? 1.0/(size + (W - i)) : x;
    if (i<size) {
      x = 1.0/ (size+1+i);
    } else if (i > (W-size-1)) {
      x = 1.0 / (size + (W - i));
    }

    for(int j=0; j<size; j++) // initialize  values
    {
      red_val += imageOut->data[cur*H+j].red;
      green_val += imageOut->data[cur*H+j].green;
      blue_val += imageOut->data[cur*H+j].blue;
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
      next++; cur+=W;
    }
    for(int j=size+1; j<H-size; j++) // Middle part
    {
      red_val += imageOut->data[next].red - imageOut->data[prev].red;
      green_val += imageOut->data[next].green - imageOut->data[prev].green;
      blue_val += imageOut->data[next].blue - imageOut->data[prev].blue;
      imageIn->data[cur].red = red_val * rec * x;
      imageIn->data[cur].green = green_val * rec * x;
      imageIn->data[cur].blue = blue_val * rec * x;
      prev++; next++; cur+=W;
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
      prev++; cur+=W;
    }
  }
}

float getNewValue(float old_value) {
  //old_value = (old_value < -1.0) ?
  if (old_value < -1)
    old_value += 257;
  if(old_value > 255)
    return 255;
  return old_value;
}

// Perform the final step, and return it as ppm.

/*PPMImage ** performNewIdeaFinalization(AccurateImage *it, AccurateImage *is, AccurateImage *im, AccurateImage *il) {
  PPMImage *iot;
  iot = (PPMImage *)malloc(sizeof(PPMImage));
  iot->data = (PPMPixel*)malloc(it->x * it->y * sizeof(PPMPixel));
  iot->x = it->x;
  iot->y = it->y;
  PPMImage *ios;
  ios = (PPMImage *)malloc(sizeof(PPMImage));
  ios->data = (PPMPixel*)malloc(it->x * it->y * sizeof(PPMPixel));
  ios->x = it->x;
  ios->y = it->y;
  PPMImage *iom;
  iom = (PPMImage *)malloc(sizeof(PPMImage));
  iom->data = (PPMPixel*)malloc(it->x * it->y * sizeof(PPMPixel));
  iom->x = it->x;
  iom->y = it->y;
  PPMImage **final = (PPMImage **)malloc(3*sizeof(PPMImage));
  final[0] = iot;
  final[1] = ios;
  final[2] = iom;
  for(int i = 0; i < it->x * it->y; i++) {
    float vtr = it->data[i].red;
    float vtg = it->data[i].green;
    float vtb = it->data[i].blue;
    float vsr = is->data[i].red;
    float vsg = is->data[i].green;
    float vsb = is->data[i].blue;
    float vmr = im->data[i].red;
    float vmg = im->data[i].green;
    float vmb = im->data[i].blue;
    float vlr = il->data[i].red;
    float vlg = il->data[i].green;
    float vlb = il->data[i].blue;
    iot->data[i].red = getNewValue(vsr-vtr);
    iot->data[i].green = getNewValue(vsg-vtg);
    iot->data[i].blue = getNewValue(vsb-vtb);
    ios->data[i].red = getNewValue(vmr-vsr);
    ios->data[i].green = getNewValue(vmg-vsg);
    ios->data[i].blue = getNewValue(vmb-vsb);
    iom->data[i].red = getNewValue(vlr-vmr);
    iom->data[i].green = getNewValue(vlg-vmg);
    iom->data[i].blue = getNewValue(vlb-vmb);
  }
  return final;
}*/
PPMImage * performNewIdeaFinalization(AccurateImage *is, AccurateImage *il) {
  PPMImage *imageOut;
  imageOut = (PPMImage *)malloc(sizeof(PPMImage));
  imageOut->data = (PPMPixel*)malloc(W * H * sizeof(PPMPixel));

  imageOut->x = W;
  imageOut->y = H;

  for(int i = 0; i < W * H; i++) {
    float value = (il->data[i].red - is->data[i].red);
    imageOut->data[i].red = getNewValue(value);
    value = (il->data[i].green - is->data[i].green);
    imageOut->data[i].green = getNewValue(value);
    value = (il->data[i].blue - is->data[i].blue);
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
  /*PPMImage **final = performNewIdeaFinalization(it2, is2, im2, il2);
	if(argc > 1) {
		writePPM("flower_tiny.ppm", final[0]);
		writePPM("flower_small.ppm", final[1]);
		writePPM("flower_medium.ppm", final[2]);
	} else {
		writeStreamPPM(stdout, final[0]);
		writeStreamPPM(stdout, final[1]);
		writeStreamPPM(stdout, final[2]);
	}*/
	return 0;
}
