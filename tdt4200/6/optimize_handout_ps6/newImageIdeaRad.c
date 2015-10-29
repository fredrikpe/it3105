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

typedef struct {
     double red,green,blue;
} AccuratePixel2;

typedef struct {
     int x, y;
     AccuratePixel2 *data;
} AccurateImageBuffer;

void freeImage(AccurateImage *image){
	free(image->data);
	free(image);
}

// Convert ppm to high precision format.
AccurateImage *convertImageToNewFormat(PPMImage *image) {
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

AccurateImageBuffer *createEmptyImageBuffer(PPMImage *image){
	AccurateImageBuffer *imageBuffer;
	imageBuffer = (AccurateImageBuffer *)malloc(sizeof(AccurateImageBuffer));
	imageBuffer->data = (AccuratePixel2*)malloc(image->x * image->y * sizeof(AccuratePixel2));
	imageBuffer->x = image->x;
	imageBuffer->y = image->y;

	return imageBuffer;
}

void addValue(AccurateImage *imageOut, AccurateImageBuffer *b, int a, int j) {
  imageOut->data[a].red += b->data[j].red;
  imageOut->data[a].blue += b->data[j].blue;
  imageOut->data[a].green += b->data[j].green;
}
void subtractValue(AccurateImage *imageOut, AccurateImageBuffer *b, int a, int j) {
  imageOut->data[a].red -= b->data[j].red;
  imageOut->data[a].blue -= b->data[j].blue;
  imageOut->data[a].green -= b->data[j].green;
}
void performNewIdeaIteration(AccurateImage *imageOut, AccurateImageBuffer *b, AccurateImage *imageIn, int size) {

  int L = size + size + 1;
  float r_L = 1.0 / L;

  int W = imageIn->x;
  int H = imageIn->y;


  for( int y=0; y<H; y++)
  {
    for(int x=0; x<W; x++)
    {
      imageOut->data[y*W + x].red = 0.0;
      imageOut->data[y*W + x].blue = 0.0;
      imageOut->data[y*W + x].green = 0.0;
    }
  }

  for( int y=0; y<H; y++)
  {
    for(int x=0; x<W; x++)
    {
      b->data[y*W + x].red = (double)imageIn->data[y*W + x].red;
      b->data[y*W + x].blue = (double)imageIn->data[y*W + x].blue;
      b->data[y*W + x].green = (double)imageIn->data[y*W + x].green;
      if (x)
      {
        if (y)
        {
          b->data[y*W + x].red += b->data[y*W + x-1].red + b->data[(y-1)*W + x].red - b->data[(y-1)*W + x-1].red;
          b->data[y*W + x].blue += b->data[y*W + x-1].blue + b->data[(y-1)*W + x].blue - b->data[(y-1)*W + x-1].blue;
          b->data[y*W + x].green += b->data[y*W + x-1].green + b->data[(y-1)*W + x].green - b->data[(y-1)*W + x-1].green;
        }
        else
        {
          b->data[y*W + x].red += b->data[y*W + x-1].red;
          b->data[y*W + x].blue += b->data[y*W + x-1].blue;
          b->data[y*W + x].green += b->data[y*W + x-1].green;
        }
      }
      else
      {
        if (y)
        {
          b->data[y*W + x].red += b->data[(y-1)*W + x].red;
          b->data[y*W + x].blue += b->data[(y-1)*W + x].blue;
          b->data[y*W + x].green += b->data[(y-1)*W + x].green;
        }
        else
        {
          b->data[y*W + x].red += b->data[(y-1)*W + x].red;
          b->data[y*W + x].blue += b->data[(y-1)*W + x].blue;
          b->data[y*W + x].green += b->data[(y-1)*W + x].green;
        }
      }
    }
  }
  for( int y=0; y<H; y++)
  {
    for(int x=0; x<W; x++)
    {
      int p1 = (y-size-1)*W + (x-size-1);
      int p2 = (y-size-1)*W + (x+size);
      int p3 = (y+size)*W + (x-size-1);
      int p4 = (y+size)*W + (x+size);

      if (x-size-1 >= 0)
      {
        if (y-size-1 >= 0)
        {
          if (x+size < W)
          {
            if (y+size < H) // all
            {
              addValue(imageOut, b, y*W + x, p1);
              subtractValue(imageOut, b, y*W + x, p2);
              subtractValue(imageOut, b, y*W + x, p3);
              addValue(imageOut, b, y*W + x, p4);
            }
            else // new 3 4
            {
              addValue(imageOut, b, y*W + x, p1);
              subtractValue(imageOut, b, y*W + x, p2);
              subtractValue(imageOut, b, y*W + x, (H-1)*W + (x-size-1));
              addValue(imageOut, b, y*W + x, (H-1)*W + (x+size));
            }
          }
          else // new 2 4
          {
            if (y+size < H)
            {
              addValue(imageOut, b, y*W + x, p1);
              subtractValue(imageOut, b, y*W + x, (y-size-1)*W + (W-1));
              subtractValue(imageOut, b, y*W + x, p3);
              addValue(imageOut, b, y*W + x, (y+size)*W + (W-1));
            }
            else // new 3 4
            {
              addValue(imageOut, b, y*W + x, p1);
              subtractValue(imageOut, b, y*W + x, (y-size-1)*W + (W-1));
              subtractValue(imageOut, b, y*W + x, (H-1)*W + (x-size-1));
              addValue(imageOut, b, y*W + x, H*W - 1);
            }
          }
        }
        else // not 1 2
        {
          if (x+size < W)
          {
            subtractValue(imageOut, b, y*W + x, p3);
            addValue(imageOut, b, y*W + x, p4);
          }
          else // new 2 4
          {
            subtractValue(imageOut, b, y*W + x, (y-size-1)*W + (W-1));
            subtractValue(imageOut, b, y*W + x, p3);
            addValue(imageOut, b, y*W + x, (y+size)*W + (W-1));
          }
        }
      }
      else // not 1 3
      {
        if (y-size-1 >= 0)
        {
          if (y+size < H)
          {
            subtractValue(imageOut, b, y*W + x, p2);
            addValue(imageOut, b, y*W + x, p4);
          }
          else // new 3 4
          {
            subtractValue(imageOut, b, y*W + x, p2);
            subtractValue(imageOut, b, y*W + x, (H-1)*W + (x-size-1));
            addValue(imageOut, b, y*W + x, (H-1)*W + (x+size));
          }
        }
        else // not 1 2
        {
          addValue(imageOut, b, y*W + x, p4);
        }
      }
    }
    //24824208674060434122114508256204708830988520457093219874699739136.0

    //printf("%.1lf\n", b->data[H*W-1]);
  }

  // Dividing
  for(int x=0; x<W; x++)
  {
    float r_x = 1.0 / L;
    if (x < size) {
      r_x = 1.0 / (size+1+x);
    } else if (x > (W-size-1)) {
      r_x = 1.0 / (size + (W - x));
    }
    for(int j=0  ; j<=size ; j++) // Start edge
    {
      float y = 1.0 / (size + 1 + j);
      imageOut->data[j*W + x].red *= y * r_x;
      imageOut->data[j*W + x].green *= y * r_x;
      imageOut->data[j*W + x].blue *= y * r_x;
    }
    for(int j=size+1; j<H-size; j++) // Middle part
    {
      imageOut->data[j*W + x].red *= r_L * r_x;
      imageOut->data[j*W + x].green *= r_L * r_x;
      imageOut->data[j*W + x].blue *= r_L * r_x;
    }
    for(int j=1; j<=size  ; j++) // End edge
    {
      float y = 1.0 / (L - j);
      imageOut->data[(H-size-1+j)*W + x].red *= y * r_x;
      imageOut->data[(H-size-1+j)*W + x].green *= y * r_x;
      imageOut->data[(H-size-1+j)*W + x].blue *= y * r_x;
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
  AccurateImageBuffer *imageBuffer2 = createEmptyImageBuffer(image);

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

  freeImage(imageUnchanged);
  freeImage(imageBuffer);
  free(imageBuffer2->data);
  free(imageBuffer2);
  freeImage(imageSmall);
  freeImage(imageBig);
  free(imageOut->data);
  free(imageOut);
  free(image->data);
  free(image);
	return 0;
}
