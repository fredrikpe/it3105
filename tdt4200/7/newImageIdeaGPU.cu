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

__global__ void convertImageToNewFormatGPU(int width, int height, float* imageUnchanged, unsigned char* originalData)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= W || y >= H) {
        return;
    }

    int i = y * W * 3 + x * 3;
    imageUnchanged[i] = (float) originalData[i];
    imageUnchanged[i + 1] = (float) originalData[i + 1];
    imageUnchanged[i + 2] = (float) originalData[i + 2];
}

// free memory of an AccurateImage
void freeImage(AccurateImage *image){
	free(image->data);
	free(image);
}

__global__ void performNewIdeaIterationGPU(float *imageOut, float *imageIn, int size, int W, int H) {
  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;
  if (x >= W || y >= H) {
      return;
  }
    // For each pixel we compute the magic number
  float sumR = 0;
  float sumG = 0;
  float sumB = 0;
  int countIncluded = 0;
  int senterY = (i+1) / H;
  int senterX = i - W * senterY;

  for(int i = -size; i <= size; i++)
  {
    for(int j = -size; j <= size; j++)
    {
      int currentX = x + i;
      int currentY = y + j;
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
			int offsetOfThePixel = (W * currentY + currentX);
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
  int i = y * W * 3 + x * 3;
  imageOut[i] = valueR;
  imageOut[i + 1] = valueG;
  imageOut[i + 2] = valueB;
}

__global__ void performNewIdeaFinalizationGPU(float* imageInSmall, float* imageInLarge, unsigned char* imageOut, int W, int H) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= W || y >= H)
        return;

    int i = y * W * 3 + x * 3;

    float value = imageInLarge[i] - imageInSmall[i];
    imageOut[i] = (int) value;
    value = imageInLarge[i+1] - imageInSmall[i+1];
    imageOut[i+1] = (int) value;
    value = imageInLarge[i+2] - imageInSmall[i+2];
    imageOut[i+2] = (int) value;
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
  dim3 dimGrid(
      ceilf((width + 31) / 32),
      ceilf((height + 31) / 32)
  );
  dim3 dimBlock(
      32,
      32
  );

  //size_t image_size = sizeof(AccurateImage);
  size_t data_size = 3*W*H*sizeof(float);
  //size_t PPMimage_size = sizeof(PPMImage);
  size_t PPMdata_size = sizeof(PPMPixel) * W * H;

  float *deviceUnchanged;
  float *deviceBuffer;
  float *deviceSmall;
  float *deviceBig;
  float *deviceOut;

  PPMImage *imageOut;
  imageOut = (PPMImage *)malloc(sizeof(PPMImage));
  imageOut->data = (PPMPixel*)malloc(image->x * image->y * sizeof(PPMPixel));

  // Allocate memory on the GPU
  cudaMalloc(&deviceOut, data_size);
  cudaMemcpy(deviceOut, image->data, PPMdata_size, cudaMemcpyHostToDevice);

  cudaMalloc(&deviceUnchanged, data_size);

  cudaMalloc(&deviceBuffer, data_size);

  cudaMalloc(&deviceSmall, data_size);

  cudaMalloc(&deviceBig, data_size);

  convertImageToNewFormatGPU<<<numBlocks, numThreads>>>();

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


  performNewIdeaFinalizationGPU<<<numBlocks, numThreads>>>(deviceSmall, deviceBig, deviceOut, W, H);
  cudaMemcpy(imageOut->data, deviceOut, PPMdata_size, cudaMemcpyDeviceToHost);
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

  performNewIdeaFinalizationGPU<<<numBlocks, numThreads>>>(deviceBig, deviceSmall, deviceOut, W, H);
  cudaMemcpy(imageOut->data, deviceOut, PPMdata_size, cudaMemcpyDeviceToHost);
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

  performNewIdeaFinalizationGPU<<<numBlocks, numThreads>>>(deviceSmall, deviceBig, deviceOut, W, H);
  cudaMemcpy(imageOut->data, deviceOut, PPMdata_size, cudaMemcpyDeviceToHost);
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
