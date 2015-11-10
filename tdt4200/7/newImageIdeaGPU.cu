#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ppmCU.h"

// Image from:
// http://7-themes.com/6971875-funny-flowers-pictures.html

typedef struct {
     float red,green,blue;
} AccuratePixel;

typedef struct {
     int x, y;
     AccuratePixel *data;
} AccurateImage;

// Convert a PPM image to a high-precision format
AccurateImage * convertImageToNewFormat(PPMImage *image) {

  AccurateImage *a = (AccurateImage*)malloc(sizeof(AccurateImage));
  a->data = (AccuratePixel*)malloc(image->x*image->y*sizeof(AccuratePixel));
  for(int i = 0; i < image->x*image->y; i++)
  {
		a->data[i].red   = (float) image->data[i].red;
		a->data[i].green = (float) image->data[i].green;
		a->data[i].blue  = (float) image->data[i].blue;
  }
  a->x = image->x;
  a->y = image->y;
  return a;
}

AccurateImage *createEmptyImage(PPMImage *image){
  AccurateImage *imageAccurate;
  imageAccurate = (AccurateImage *)malloc(sizeof(AccurateImage));
  imageAccurate->data = (AccuratePixel*)malloc(image->x * image->y * sizeof(AccuratePixel));
  imageAccurate->x = image->x;
  imageAccurate->y = image->y;

  return imageAccurate;
}

// Convert a high-precision format to a PPM image
PPMImage *convertNewFormatToPPM(AccurateImage *image) {
	// Make a copy
	PPMImage *imagePPM;
	imagePPM = (PPMImage *)malloc(sizeof(PPMImage));
	imagePPM->data = (PPMPixel*)malloc(image->x * image->y * sizeof(PPMPixel));
	for(int i = 0; i < image->x * image->y; i++) {
		imagePPM->data[i].red   = (unsigned char) image->data[i].red;
		imagePPM->data[i].green = (unsigned char) image->data[i].green;
		imagePPM->data[i].blue  = (unsigned char) image->data[i].blue;
	}
	imagePPM->x = image->x;
	imagePPM->y = image->y;

	return imagePPM;
}

// free memory of an AccurateImage
void freeImage(AccurateImage *image){
	free(image->data);
	free(image);
}

__global__ void performNewIdeaIterationGPU(float *imageOut, float *imageIn, int size, int W, int H) {


  // This assumes that (channels * width * height) is divisible by (number of blocks)
  int pixels_per_column = (W*H) / gridDim.x;

  // The start index of a single thread is the start location of the block,
  // plus the thread's index within the block
  int start = (blockIdx.x * pixels_per_column) + threadIdx.x;

  // Run until the start index of the next block
  int stop =  ((blockIdx.x + 1) * pixels_per_column);


  for(int i = start; i < stop; i += blockDim.x) {
    // For each pixel we compute the magic number
    float sumR = 0;
    float sumG = 0;
    float sumB = 0;
    int countIncluded = 0;
    int senterY = (i+1) / H;
    int senterX = i - W * senterY;

    for(int x = -size; x <= size; x++) {

	for(int y = -size; y <= size; y++) {
		int currentX = senterX + x;
		int currentY = senterY + y;
			// Check if we are outside the bounds
			if(currentX < 0)
				continue;
			if(currentX >= W)
				continue;
			if(currentY < 0)
				continue;
			if(currentY >= H)
				continue;

				// Now we can begin
				int numberOfValuesInEachRow = W;
				int offsetOfThePixel = (numberOfValuesInEachRow * currentY + currentX);
				sumR += imageIn[offsetOfThePixel];
				sumG += imageIn[offsetOfThePixel+1];
				sumB += imageIn[offsetOfThePixel+2];

				// Keep track of how many values we have included
				countIncluded++;
			}
		}

		// Now we compute the final value for all colours
		float valueR = sumR / countIncluded;
		float valueG = sumG / countIncluded;
		float valueB = sumB / countIncluded;

		// Update the output image
		int numberOfValuesInEachRow = W; // R, G and B
		int offsetOfThePixel = (numberOfValuesInEachRow * senterY + senterX);
		imageOut[offsetOfThePixel] = valueR;
		imageOut[offsetOfThePixel+1] = valueG;
		imageOut[offsetOfThePixel+2] = valueB;
	}
}

// Perform the final step, and save it as a ppm in imageOut
__global__ void performNewIdeaFinalizationGPU(float *imageInSmall, float *imageInLarge, float *imageOut)
{
  int element_count = sizeof(imageInSmall)/4;
  int pixels_per_column = element_count / gridDim.x;
  int start = (blockIdx.x * pixels_per_column) + threadIdx.x;
  int stop =  ((blockIdx.x + 1) * pixels_per_column);


  for(int i = start; i < stop; i += blockDim.x)
  {
    float value = imageInLarge[i] - imageInSmall[i];
    imageOut[i] = (int) value;
    value = imageInLarge[i+1] - imageInSmall[i+1];
    imageOut[i+1] = (int) value;
    value = imageInLarge[i+2] - imageInSmall[i+2];
    imageOut[i+2] = (int) value;
  }
}

int main(int argc, char** argv) {

  PPMImage *image;

  if(argc > 1) {
    image = readPPM("flower.ppm");
  } else {
    image = readStreamPPM(stdin);
  }

  int W = image->x;
  int H = image->y;
  const int numBlocks = 5;				// Total number of blocks
  const int numThreads = 32;				// Threads per block. 32 is the recommended size by nvidia,

  //size_t image_size = sizeof(AccurateImage);
  size_t data_size = 3*W*H*sizeof(float);
  //size_t PPMimage_size = sizeof(PPMImage);
  size_t PPMdata_size = sizeof(PPMPixel) * W * H;

  AccurateImage *imageUnchanged = convertImageToNewFormat(image); // save the unchanged image from input image
  AccurateImage *imageDummy = createEmptyImage(image);

  float *deviceUnchanged;
  float *deviceBuffer;
  float *deviceSmall;
  float *deviceBig;
  float *deviceOut;

  PPMImage *imageOut;
  imageOut = (PPMImage *)malloc(sizeof(PPMImage));
  imageOut->data = (PPMPixel*)malloc(image->x * image->y * sizeof(PPMPixel));



  // Allocate memory on the GPU
  cudaMalloc((void**) &deviceUnchanged, data_size);
  cudaMemcpy(deviceUnchanged, imageUnchanged->data, data_size, cudaMemcpyHostToDevice);

  cudaMalloc((void**) &deviceBuffer, data_size);
  cudaMemcpy(deviceBuffer, imageDummy->data, data_size, cudaMemcpyHostToDevice);

  cudaMalloc((void**) &deviceSmall, data_size);
  cudaMemcpy(deviceSmall, imageDummy->data, data_size, cudaMemcpyHostToDevice);

  cudaMalloc((void**) &deviceBig, data_size);
  cudaMemcpy(deviceBig, imageDummy->data, data_size, cudaMemcpyHostToDevice);

  cudaMalloc((void**) &deviceOut, PPMdata_size);
  cudaMemcpy(deviceOut, imageDummy->data, PPMdata_size, cudaMemcpyHostToDevice);


  // Do iterations


  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceSmall, deviceUnchanged, 2, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceBuffer, deviceSmall, 2, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceSmall, deviceBuffer, 2, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceBuffer, deviceSmall, 2, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceSmall, deviceBuffer, 2, W, H);

  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceBig, deviceUnchanged, 3, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceBuffer, deviceBig, 3, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceBig, deviceBuffer, 3, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceBuffer, deviceBig, 3, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceBig, deviceBuffer, 3, W, H);


  performNewIdeaFinalizationGPU<<<numBlocks, numThreads>>>(deviceSmall, deviceBig, deviceOut);
  cudaMemcpy(image->data, deviceOut, PPMdata_size, cudaMemcpyDeviceToHost);
  if(argc > 1) {
    writePPM("flower_medium.ppm", image);
  } else {
    writeStreamPPM(stdout, image);
  }

  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceSmall, deviceUnchanged, 5, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceBuffer, deviceSmall, 5, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceSmall, deviceBuffer, 5, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceBuffer, deviceSmall, 5, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceSmall, deviceBuffer, 5, W, H);

  performNewIdeaFinalizationGPU<<<numBlocks, numThreads>>>(deviceBig, deviceSmall, deviceOut);
  //cudaMemcpy(image, deviceOut, PPMimage_size, cudaMemcpyDeviceToHost);
  cudaMemcpy(image->data, deviceOut, PPMdata_size, cudaMemcpyDeviceToHost);
  if(argc > 1) {
    writePPM("flower_medium.ppm", image);
  } else {
    writeStreamPPM(stdout, image);
  }

  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceBig, deviceUnchanged, 8, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceBuffer, deviceBig, 8, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceBig, deviceBuffer, 8, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceBuffer, deviceBig, 8, W, H);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(deviceBig, deviceBuffer, 8, W, H);

  performNewIdeaFinalizationGPU<<<numBlocks, numThreads>>>(deviceSmall, deviceBig, deviceOut);
  //cudaMemcpy(image, deviceOut, PPMimage_size, cudaMemcpyDeviceToHost);
  cudaMemcpy(image->data, deviceOut, PPMdata_size, cudaMemcpyDeviceToHost);
  if(argc > 1) {
    writePPM("flower_medium.ppm", image);
  } else {
    writeStreamPPM(stdout, image);
  }

  cudaFree(deviceUnchanged);
  cudaFree(deviceBuffer);
  cudaFree(deviceSmall);
  cudaFree(deviceBig);
  cudaFree(deviceOut);
  free(image->data);
  free(image);
  return 0;
}
