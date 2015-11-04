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
__global__ void convertImageToNewFormatGPU(AccurateImage *imageNew, PPMImage *image) {
  int pixels_per_column = element_count / gridDim.x;
  int start = (blockIdx.x * pixels_per_column) + threadIdx.x;
  int stop =  ((blockIdx.x + 1) * pixels_per_column);
  for(int i = start; i < stop; i += blockDim.x)
  {
		imageNew->data[i].red   = (float) image->data[i].red;
		imageNew->data[i].green = (float) image->data[i].green;
		imageNew->data[i].blue  = (float) image->data[i].blue;
	}
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

__global__ void performNewIdeaIterationGPU(AccurateImage *imageOut, AccurateImage *imageIn, int size) {
  // This assumes that (channels * width * height) is divisible by (number of blocks)
  int pixels_per_column = element_count / gridDim.x;

  // The start index of a single thread is the start location of the block,
  // plus the thread's index within the block
  int start = (blockIdx.x * pixels_per_column) + threadIdx.x;

  // Run until the start index of the next block
  int stop =  ((blockIdx.x + 1) * pixels_per_column);

  int W = imageIn->x;
  int H = imageIn->y;


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
				int numberOfValuesInEachRow = imageIn->x;
				int offsetOfThePixel = (numberOfValuesInEachRow * currentY + currentX);
				sumR += imageIn->data[offsetOfThePixel].red;
				sumG += imageIn->data[offsetOfThePixel].green;
				sumB += imageIn->data[offsetOfThePixel].blue;

				// Keep track of how many values we have included
				countIncluded++;
			}
		}

		// Now we compute the final value for all colours
		float valueR = sumR / countIncluded;
		float valueG = sumG / countIncluded;
		float valueB = sumB / countIncluded;

		// Update the output image
		int numberOfValuesInEachRow = imageOut->x; // R, G and B
		int offsetOfThePixel = (numberOfValuesInEachRow * senterY + senterX);
		imageOut->data[offsetOfThePixel].red = valueR;
		imageOut->data[offsetOfThePixel].green = valueG;
		imageOut->data[offsetOfThePixel].blue = valueB;
	}
}

// Perform the final step, and save it as a ppm in imageOut
__global__ void performNewIdeaFinalizationGPU(AccurateImage *imageInSmall, AccurateImage *imageInLarge, PPMImage *imageOut)
{
  int pixels_per_column = element_count / gridDim.x;
  int start = (blockIdx.x * pixels_per_column) + threadIdx.x;
  int stop =  ((blockIdx.x + 1) * pixels_per_column);

  imageOut->x = imageInSmall->x;
  imageOut->y = imageInSmall->y;

  for(int i = start; i < stop; i += blockDim.x) {
  {
    float value = imageInLarge->data[i].red - imageInSmall->data[i].red;
    imageOut->data[i].red = (int) value;
    value = imageInLarge->data[i].green - imageInSmall->data[i].green;
    imageOut->data[i].green = (int) value;
    value = imageInLarge->data[i].blue - imageInSmall->data[i].blue;
    imageOut->data[i].blue = (int) value;
  }
}

__global__ void fiveIterationsGPU(AccurateImage *imageNew, AccurateImage *imageUnchanged, AccurateImage *imageBuffer, int size)
{
  performNewIdeaIterationGPU<<<5, 32>>>(imageNew, imageUnchanged, size);
  performNewIdeaIterationGPU<<<5, 32>>>(imageBuffer, imageNew, size);
  performNewIdeaIterationGPU<<<5, 32>>>(imageNew, imageBuffer, size);
  performNewIdeaIterationGPU<<<5, 32>>>(imageBuffer, imageNew, size);
  performNewIdeaIterationGPU<<<5, 32>>>(imageNew, imageBuffer, size);
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

  size_t image_size = sizeof(AccurateImage);
  size_t data_size = sizeof(AccuratePixel) * W * H;
  size_t PPMimage_size = sizeof(PPMImage);
  size_t PPMdata_size = sizeof(PPMPixel) * W * H;

  AccurateImage *imageUnchanged;
  AccurateImage *imageBuffer;
  AccurateImage *imageSmall;
  AccurateImage *imageBig;
  PPMImage *imageOut;

  // Allocate memory on the GPU
  cudaMalloc((void**) &imageUnchanged, image_size);
  cudaMalloc((void**) &imageUnchanged.data, data_size);
  cudaMalloc((void**) &imageBuffer, image_size);
  cudaMalloc((void**) &imageBuffer.data, data_size);
  cudaMalloc((void**) &imageSmall, image_size);
  cudaMalloc((void**) &imageSmall.data, data_size);
  cudaMalloc((void**) &imageBig, image_size);
  cudaMalloc((void**) &imageBig.data, data_size);
  cudaMalloc((void**) &imageOut, PPMimage_size);
  cudaMalloc((void**) &imageOut.data, PPMdata_size);

  convertImageToNewFormatGPU<<<numBlocks, numThreads>>>(imageUnchanged, image);

  fiveIterationsGPU<<<numBlocks, numThreads>>>(imageSmall, imageUnchanged, imageBuffer, 2);
  fiveIterationsGPU<<<numBlocks, numThreads>>>(imageBig, imageUnchanged, imageBuffer, 3);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(imageSmall, imageBig, imageOut);
  cudaMemcpy(image, imageOut, PPMimage_size, cudaMemcpyDeviceToHost);
  if(argc > 1) {
    writePPM("flower_medium.ppm", image);
  } else {
    writeStreamPPM(stdout, image);
  }

  fiveIterationsGPU<<<numBlocks, numThreads>>>(imageSmall, imageUnchanged, imageBuffer, 5);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(imageBig, imageSmall, imageOut);
  cudaMemcpy(image, imageOut, PPMimage_size, cudaMemcpyDeviceToHost);
  if(argc > 1) {
    writePPM("flower_medium.ppm", image);
  } else {
    writeStreamPPM(stdout, image);
  }

  fiveIterationsGPU<<<numBlocks, numThreads>>>(imageBig, imageUnchanged, imageBuffer, 8);
  performNewIdeaIterationGPU<<<numBlocks, numThreads>>>(imageSmall, imageBig, imageOut);
  cudaMemcpy(image, imageOut, PPMimage_size, cudaMemcpyDeviceToHost);
  if(argc > 1) {
    writePPM("flower_medium.ppm", image);
  } else {
    writeStreamPPM(stdout, image);
  }

  cudaFree(imageUnchanged);
  cudaFree(imageBuffer);
  cudaFree(imageSmall);
  cudaFree(imageBig);
  cudaFree(imageOut);
  cudaFree(imageUnchanged->data);
  cudaFree(imageBuffer->data);
  cudaFree(imageSmall->data);
  cudaFree(imageBig->data);
  cudaFree(imageOut->data);
  free(image->data);
  free(image);
	return 0;
}
