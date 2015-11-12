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

__global__ void convertImageToNewFormatGPU(float* imageUnchanged, unsigned char* image, int W, int H)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= W || y >= H) {
        return;
    }
    int i = (W * y + x) * 3;
    imageUnchanged[i]   = __uint2float_rd(image[i]);
    imageUnchanged[i + 1] = __uint2float_rd(image[i + 1]);
    imageUnchanged[i + 2] = __uint2float_rd(image[i + 2]);
}

// free memory of an AccurateImage
void freeImage(AccurateImage *image){
	free(image->data);
	free(image);
}
__global__ void performNewIdeaIterationGPU(float* imageOut, float* imageIn, int size, int W, int H) {
    int X = blockIdx.x * blockDim.x + threadIdx.x;
    int Y = blockIdx.y * blockDim.y + threadIdx.y;
    if (X >= W || Y >= H) {
        return;
    }

    float sumR = 0.0;
    float sumG = 0.0;
    float sumB = 0.0;
    int countIncluded = 0;
    for(int x = -size; x <= size; x++) {

        for(int y = -size; y <= size; y++) {
            int currentX = X + x;
            int currentY = Y + y;

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
            int i = (W * currentY + currentX) * 3;
            sumR += imageIn[i];
            sumG += imageIn[i + 1];
            sumB += imageIn[i + 2];

            // Keep track of how many values we have included
            countIncluded++;
        }

    }
    float valueR = sumR / countIncluded;
    float valueG = sumG / countIncluded;
    float valueB = sumB / countIncluded;

    int i = (W * Y + X) * 3;
    imageOut[i] = valueR;
    imageOut[i + 1] = valueG;
    imageOut[i + 2] = valueB;
}

__global__ void performNewIdeaFinalizationGPU(float* imageInSmall, float* imageInLarge, unsigned char* imageOut, int W, int H) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= W || y >= H) {
        return;
    }
    int i = (W * y + x) * 3;

    float value = (imageInLarge[i] - imageInSmall[i]);
    if(value > 255.0f)
        imageOut[i] = 255;
    else if (value < -1.0f) {
        value = 257.0f+value;
        if(value > 255.0f)
            imageOut[i] = 255;
        else
            imageOut[i] = floorf(value);
    } else if (value > -1.0f && value < 0.0f) {
        imageOut[i] = 0;
    } else {
        imageOut[i] = floorf(value);
    }
    value = (imageInLarge[i+1] - imageInSmall[i+1]);
    if(value > 255.0f)
        imageOut[i+1] = 255;
    else if (value < -1.0f) {
        value = 257.0f+value;
        if(value > 255.0f)
            imageOut[i+1] = 255;
        else
            imageOut[i+1] = floorf(value);
    } else if (value > -1.0f && value < 0.0f) {
        imageOut[i+1] = 0;
    } else {
        imageOut[i+1] = floorf(value);
    }

    value = (imageInLarge[i+2] - imageInSmall[i+2]);
    if(value > 255.0f)
        imageOut[i+2] = 255;
    else if (value < -1.0f) {
        value = 257.0f+value;
        if(value > 255.0f)
            imageOut[i+2] = 255;
        else
            imageOut[i+2] = floorf(value);
    } else if (value > -1.0f && value < 0.0f) {
        imageOut[i+2] = 0;
    } else {
        imageOut[i+2] = floorf(value);
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
  dim3 blockDim(
      ceilf((W + 31) / 32),
      ceilf((H + 31) / 32)
  );
  dim3 threadDim(
      32,
      32
  );

  size_t data_size = 3*W*H*sizeof(float);
  size_t PPMdata_size = sizeof(unsigned char) * W * H;

  float *deviceUnchanged;
  float *deviceBuffer;
  float *deviceSmall;
  float *deviceBig;
  unsigned char *deviceOut;

  PPMImage *imageOut;
  imageOut = (PPMImage *)malloc(sizeof(PPMImage));
  imageOut->data = (PPMPixel*)malloc(image->x * image->y * sizeof(PPMPixel));
  imageOut->x = W;
  imageOut->y = H;
  // Allocate memory on the GPU
  cudaMalloc(&deviceOut, PPMdata_size);
  cudaMalloc(&deviceUnchanged, data_size);
  cudaMalloc(&deviceBuffer, data_size);
  cudaMalloc(&deviceSmall, data_size);
  cudaMalloc(&deviceBig, data_size);

  cudaMemcpy(deviceOut, image->data, PPMdata_size, cudaMemcpyHostToDevice);

  convertImageToNewFormatGPU<<<blockDim, threadDim>>>(deviceUnchanged, deviceOut, W, H);

  // Do iterations
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceSmall, deviceUnchanged, 2, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceBuffer, deviceSmall, 2, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceSmall, deviceBuffer, 2, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceBuffer, deviceSmall, 2, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceSmall, deviceBuffer, 2, W, H);

  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceBig, deviceUnchanged, 3, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceBuffer, deviceBig, 3, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceBig, deviceBuffer, 3, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceBuffer, deviceBig, 3, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceBig, deviceBuffer, 3, W, H);


  performNewIdeaFinalizationGPU<<<blockDim, threadDim>>>(deviceSmall, deviceBig, deviceOut, W, H);
  cudaMemcpy(imageOut->data, deviceOut, PPMdata_size, cudaMemcpyDeviceToHost);
  if(argc > 1) {
    writePPM("flower_tiny.ppm", imageOut);
  } else {
    writeStreamPPM(stdout, imageOut);
  }

  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceSmall, deviceUnchanged, 5, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceBuffer, deviceSmall, 5, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceSmall, deviceBuffer, 5, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceBuffer, deviceSmall, 5, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceSmall, deviceBuffer, 5, W, H);

  performNewIdeaFinalizationGPU<<<blockDim, threadDim>>>(deviceBig, deviceSmall, deviceOut, W, H);
  cudaMemcpy(imageOut->data, deviceOut, PPMdata_size, cudaMemcpyDeviceToHost);
  if(argc > 1) {
    writePPM("flower_small.ppm", imageOut);
  } else {
    writeStreamPPM(stdout, imageOut);
  }

  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceBig, deviceUnchanged, 8, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceBuffer, deviceBig, 8, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceBig, deviceBuffer, 8, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceBuffer, deviceBig, 8, W, H);
  performNewIdeaIterationGPU<<<blockDim, threadDim>>>(deviceBig, deviceBuffer, 8, W, H);

  performNewIdeaFinalizationGPU<<<blockDim, threadDim>>>(deviceSmall, deviceBig, deviceOut, W, H);
  cudaMemcpy(imageOut->data, deviceOut, PPMdata_size, cudaMemcpyDeviceToHost);
  if(argc > 1) {
    writePPM("flower_medium.ppm", imageOut);
  } else {
    writeStreamPPM(stdout, imageOut);
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
