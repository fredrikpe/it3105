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

void freeImage(AccurateImage *image){
	free(image->data);
	free(image);
}

// Convert ppm to high precision format.
AccurateImage *convertImageToNewFormat(PPMImage *image) {
	// Make a copy

  int W = image->x;
  int H = image->y;

	AccurateImage *imageAccurate;
	imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));
	imageAccurate->data = (AccuratePixel*)malloc(W * H * sizeof(AccuratePixel));

	for(int i = 0; i < W * H; i++) {
    imageAccurate->data[i].red   = image->data[i].red;
    imageAccurate->data[i].green = image->data[i].green;
    imageAccurate->data[i].blue  = image->data[i].blue;
	}
	imageAccurate->x = W;
	imageAccurate->y = H;

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

void performNewIdeaIteration(AccurateImage *imageOut, AccurateImage *imageBuffer, AccurateImage *imageIn, int size) {

  int L = size + size + 1;
  float rec = 1.0 / L;

  int W = imageIn->x;
  int H = imageIn->y;

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
      imageBuffer->data[cur].red = red_val;
      imageBuffer->data[cur].green = green_val;
      imageBuffer->data[cur].blue = blue_val;
      cur+=H; next++;
    }
    for(int j=size+1; j<W-size; j++) // middle part
    {
      red_val += imageIn->data[next].red - imageIn->data[prev].red;
      green_val += imageIn->data[next].green - imageIn->data[prev].green;
      blue_val += imageIn->data[next].blue - imageIn->data[prev].blue;

      imageBuffer->data[cur].red = red_val;
      imageBuffer->data[cur].green = green_val;
      imageBuffer->data[cur].blue = blue_val;
      cur+=H; next++; prev++;
    }
    for(int j=1; j<=size; j++) // end edge
    {
      red_val -= imageIn->data[prev].red;
      green_val -= imageIn->data[prev].green;
      blue_val -= imageIn->data[prev].blue;
      imageBuffer->data[cur].red = red_val;
      imageBuffer->data[cur].green = green_val;
      imageBuffer->data[cur].blue = blue_val;
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
      red_val += imageBuffer->data[cur*H+j].red;
      green_val += imageBuffer->data[cur*H+j].green;
      blue_val += imageBuffer->data[cur*H+j].blue;
    }
    for(int j=0  ; j<=size ; j++) // Start edge
    {
      float y = 1.0 / (size + 1 + j);
      red_val += imageBuffer->data[next].red;
      green_val += imageBuffer->data[next].green;
      blue_val += imageBuffer->data[next].blue;
      imageOut->data[cur].red = red_val * y * x;
      imageOut->data[cur].green = green_val * y * x;
      imageOut->data[cur].blue = blue_val * y * x;
      next++; cur+=W;
    }
    for(int j=size+1; j<H-size; j++) // Middle part
    {
      red_val += imageBuffer->data[next].red - imageBuffer->data[prev].red;
      green_val += imageBuffer->data[next].green - imageBuffer->data[prev].green;
      blue_val += imageBuffer->data[next].blue - imageBuffer->data[prev].blue;
      imageOut->data[cur].red = red_val * rec * x;
      imageOut->data[cur].green = green_val * rec * x;
      imageOut->data[cur].blue = blue_val * rec * x;
      prev++; next++; cur+=W;
    }
    for(int j=1; j<=size  ; j++) // End edge
    {
      float y = 1.0 / (L - j);
      red_val -= imageBuffer->data[prev].red;
      green_val -= imageBuffer->data[prev].green;
      blue_val -= imageBuffer->data[prev].blue;
      imageOut->data[cur].red = red_val * y * x;
      imageOut->data[cur].green = green_val * y * x;
      imageOut->data[cur].blue = blue_val * y * x;
      prev++; cur+=W;
    }
  }
}

/*float getNewValue(float old_value) {
  if (old_value < -1)
    old_value += 257;
  if(old_value > 255)
    return 255;
  return old_value;
}
PPMImage * performNewIdeaFinalization(AccurateImage *is, AccurateImage *il, PPMImage *imageOut) {
  int W = is->x;
  int H = is->y;

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
}*/
// Perform the final step, and save it as a ppm in imageOut
void performNewIdeaFinalization(AccurateImage *imageInSmall, AccurateImage *imageInLarge, PPMImage *imageOut) {

	imageOut->x = imageInSmall->x;
	imageOut->y = imageInSmall->y;

	for(int i = 0; i < imageInSmall->x * imageInSmall->y; i++) {
		float value = (imageInLarge->data[i].red - imageInSmall->data[i].red);

		if(value > 255)
			imageOut->data[i].red = 255;
		else if (value < -1) {
			value = 257.0+value;
			if(value > 255)
				imageOut->data[i].red = 255;
			else
				imageOut->data[i].red = floor(value);
		} else if (value > -1 && value < 0) {
			imageOut->data[i].red = 0;
		}  else {
			imageOut->data[i].red = floor(value);
		}

		value = (imageInLarge->data[i].green - imageInSmall->data[i].green);
		if(value > 255)
			imageOut->data[i].green = 255;
		else if (value < -1) {
			value = 257.0+value;
			if(value > 255)
				imageOut->data[i].green = 255;
			else
				imageOut->data[i].green = floor(value);
		} else if (value > -1 && value < 0) {
			imageOut->data[i].green = 0;
		} else {
			imageOut->data[i].green = floor(value);
		}

		value = (imageInLarge->data[i].blue - imageInSmall->data[i].blue);
		if(value > 255)
			imageOut->data[i].blue = 255;
		else if (value < -1) {
			value = 257.0+value;
			if(value > 255)
				imageOut->data[i].blue = 255;
			else
				imageOut->data[i].blue = floor(value);
		} else if (value > -1 && value < 0) {
			imageOut->data[i].blue = 0;
		} else {
			imageOut->data[i].blue = floor(value);
		}
	}

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
  AccurateImage *imageBuffer2 = createEmptyImage(image);

  AccurateImage *imageSmall = createEmptyImage(image);
  AccurateImage *imageBig = createEmptyImage(image);

  PPMImage *imageOut;
  imageOut = (PPMImage *)malloc(sizeof(PPMImage));
  imageOut->data = (PPMPixel*)malloc(image->x * image->y * sizeof(PPMPixel));

  // Process the tiny case:
  performNewIdeaIteration(imageSmall, imageBuffer2, imageUnchanged, 2);
  performNewIdeaIteration(imageBuffer, imageBuffer2, imageSmall, 2);
  performNewIdeaIteration(imageSmall, imageBuffer2, imageBuffer, 2);
  performNewIdeaIteration(imageBuffer, imageBuffer2, imageSmall, 2);
  performNewIdeaIteration(imageSmall, imageBuffer2, imageBuffer, 2);

  // Process the small case:
  performNewIdeaIteration(imageBig, imageBuffer2, imageUnchanged,3);
  performNewIdeaIteration(imageBuffer, imageBuffer2, imageBig,3);
  performNewIdeaIteration(imageBig, imageBuffer2, imageBuffer,3);
  performNewIdeaIteration(imageBuffer, imageBuffer2, imageBig,3);
  performNewIdeaIteration(imageBig, imageBuffer2, imageBuffer,3);

  // save tiny case result
  performNewIdeaFinalization(imageSmall,  imageBig, imageOut);
  if(argc > 1) {
    writePPM("flower_tiny.ppm", imageOut);
  } else {
    writeStreamPPM(stdout, imageOut);
  }

  // Process the medium case:
  performNewIdeaIteration(imageSmall, imageBuffer2, imageUnchanged, 5);
  performNewIdeaIteration(imageBuffer, imageBuffer2, imageSmall, 5);
  performNewIdeaIteration(imageSmall, imageBuffer2, imageBuffer, 5);
  performNewIdeaIteration(imageBuffer, imageBuffer2, imageSmall, 5);
  performNewIdeaIteration(imageSmall, imageBuffer2, imageBuffer, 5);

  // save small case
  performNewIdeaFinalization(imageBig,  imageSmall,imageOut);
  if(argc > 1) {
    writePPM("flower_small.ppm", imageOut);
  } else {
    writeStreamPPM(stdout, imageOut);
  }

  // process the large case
  performNewIdeaIteration(imageBig, imageBuffer2, imageUnchanged, 8);
  performNewIdeaIteration(imageBuffer, imageBuffer2, imageBig, 8);
  performNewIdeaIteration(imageBig, imageBuffer2, imageBuffer, 8);
  performNewIdeaIteration(imageBuffer, imageBuffer2, imageBig, 8);
  performNewIdeaIteration(imageBig, imageBuffer2, imageBuffer, 8);

  // save the medium case
  performNewIdeaFinalization(imageSmall,  imageBig, imageOut);
  if(argc > 1) {
    writePPM("flower_medium.ppm", imageOut);
  } else {
    writeStreamPPM(stdout, imageOut);
  }

  /*

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
  */
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
