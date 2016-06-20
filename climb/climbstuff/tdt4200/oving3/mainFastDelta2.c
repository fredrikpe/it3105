#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "ppm.h"

// Image from:
// http://7-themes.com/6971875-funny-flowers-pictures.html

typedef double storage_t;
typedef double compute_t;

typedef struct {
     storage_t red,green,blue;
} AccuratePixel;

typedef struct {
     int x, y;
     AccuratePixel *data;
} AccurateImage;


// Store each color independently
typedef struct {
     int x, y;
     storage_t *colors[3];
} AccurateImagePlanes;



// Convert a PPM image to a high-precision format 
AccurateImagePlanes *convertImageToNewPlaneFormat(PPMImage *image) {
	// Make a copy
	AccurateImagePlanes *imageAccuratePlanes;
	imageAccuratePlanes = (AccurateImagePlanes *)malloc(sizeof(AccurateImagePlanes));
	imageAccuratePlanes->colors[0] = (storage_t*)malloc(image->x * image->y * sizeof(storage_t));
	imageAccuratePlanes->colors[1] = (storage_t*)malloc(image->x * image->y * sizeof(storage_t));
	imageAccuratePlanes->colors[2] = (storage_t*)malloc(image->x * image->y * sizeof(storage_t));
	for(int i = 0; i < image->x * image->y; i++) {
		imageAccuratePlanes->colors[0][i] = (storage_t) image->data[i].red;
		imageAccuratePlanes->colors[1][i] = (storage_t) image->data[i].green;
		imageAccuratePlanes->colors[2][i] = (storage_t) image->data[i].blue;
	}
	imageAccuratePlanes->x = image->x;
	imageAccuratePlanes->y = image->y;
	
	return imageAccuratePlanes;
}

void performNewIdeaIteration(AccurateImagePlanes *imageOut, AccurateImagePlanes *imageIn, int colourType, int size) {
	
	// Remove the branches for most cases
	// First do the borders, then do the center region 
	
	// Do the the boarders: top
	// Also, allocate a work buffer for computations values
	compute_t *lineBuffer = (compute_t*)malloc( imageIn->x * sizeof(compute_t));
	for(int centerY = 0; centerY < size+1; centerY++) {
		for(int centerX = 0; centerX < imageIn->x; centerX++) {
			// For each pixel we compute the magic number
			compute_t sum = 0;
			int countIncluded = 0;
			for(int y = -size; y <= size; y++) {
				for(int x = -size; x <= size; x++) {
					int currentX = centerX + x;
					int currentY = centerY + y;
					
					// Check if we are outside the bounds
					if(currentX < 0)
						continue;
					if(currentX >= imageIn->x)
						continue;
					if(currentY < 0)
						continue;
					if(currentY >= imageIn->y)
						continue;
					
					// Now we can begin
					int numberOfValuesInEachRow = imageIn->x; 
					int offsetOfThePixel = (numberOfValuesInEachRow * currentY + currentX);
					sum += imageIn->colors[colourType][offsetOfThePixel];
				
					// Keep track of how many values we have included
					countIncluded++;
				}
			
			}
			
			// Now store the intermidiate computation
			lineBuffer[centerX] = sum;
			
			// Now we compute the final value
			storage_t value = (storage_t) (sum / countIncluded);
			
	
			// Update the output image
			int numberOfValuesInEachRow = imageOut->x;
			int offsetOfThePixel = (numberOfValuesInEachRow * centerY + centerX);
			imageOut->colors[colourType][offsetOfThePixel] = value;
			
		}
	}
	
	// Do the the boarders: bottom
	for(int centerY = imageIn->y-size; centerY < imageIn->y; centerY++) {
		for(int centerX = 0; centerX < imageIn->x; centerX++) {
			// For each pixel we compute the magic number
			compute_t sum = 0;
			int countIncluded = 0;
			for(int y = -size; y <= size; y++) {
				for(int x = -size; x <= size; x++) {
					int currentX = centerX + x;
					int currentY = centerY + y;
					
					// Check if we are outside the bounds
					if(currentX < 0)
						continue;
					if(currentX >= imageIn->x)
						continue;
					if(currentY < 0)
						continue;
					if(currentY >= imageIn->y)
						continue;
					
					// Now we can begin
					int numberOfValuesInEachRow = imageIn->x; 
					int offsetOfThePixel = (numberOfValuesInEachRow * currentY + currentX);
					sum += imageIn->colors[colourType][offsetOfThePixel];
				
					// Keep track of how many values we have included
					countIncluded++;
				}
			
			}
			
			// Now we compute the final value
			storage_t value = (storage_t) (sum / countIncluded);
			
	
			// Update the output image
			int numberOfValuesInEachRow = imageOut->x;
			int offsetOfThePixel = (numberOfValuesInEachRow * centerY + centerX);
			imageOut->colors[colourType][offsetOfThePixel] = value;
		}
	}
	
	// Do the the boarders: left
	for(int centerY = size+1; centerY < (imageIn->y-size); centerY++) {
		for(int centerX = 0; centerX < size+1; centerX++) {
			// For each pixel we compute the magic number
			compute_t sum = 0;
			int countIncluded = 0;
			for(int y = -size; y <= size; y++) {
				for(int x = -size; x <= size; x++) {
					int currentX = centerX + x;
					int currentY = centerY + y;
					
					// Check if we are outside the bounds
					if(currentX < 0)
						continue;
					if(currentX >= imageIn->x)
						continue;
					if(currentY < 0)
						continue;
					if(currentY >= imageIn->y)
						continue;
					
					// Now we can begin
					int numberOfValuesInEachRow = imageIn->x; 
					int offsetOfThePixel = (numberOfValuesInEachRow * currentY + currentX);
					sum += imageIn->colors[colourType][offsetOfThePixel];
				
					// Keep track of how many values we have included
					countIncluded++;
				}
			
			}
			
			// Now we compute the final value
			storage_t value = (storage_t) (sum / countIncluded);
	
			// Update the output image
			int numberOfValuesInEachRow = imageOut->x;
			int offsetOfThePixel = (numberOfValuesInEachRow * centerY + centerX);
			imageOut->colors[colourType][offsetOfThePixel] = value;
		}
	}
	
	// Do the the boarders: right
	for(int centerY = size+1; centerY < (imageIn->y-size); centerY++) {
		for(int centerX = (imageIn->x - size); centerX < imageIn->x; centerX++) {
			// For each pixel we compute the magic number
			compute_t sum = 0;
			int countIncluded = 0;
			for(int y = -size; y <= size; y++) {
				for(int x = -size; x <= size; x++) {
					int currentX = centerX + x;
					int currentY = centerY + y;
					
					// Check if we are outside the bounds
					if(currentX < 0)
						continue;
					if(currentX >= imageIn->x)
						continue;
					if(currentY < 0)
						continue;
					if(currentY >= imageIn->y)
						continue;
					
					// Now we can begin
					int numberOfValuesInEachRow = imageIn->x; 
					int offsetOfThePixel = (numberOfValuesInEachRow * currentY + currentX);
					sum += imageIn->colors[colourType][offsetOfThePixel];
				
					// Keep track of how many values we have included
					countIncluded++;
				}
			
			}
			
			// Now we compute the final value
			storage_t value = (storage_t) (sum / countIncluded);
	
			// Update the output image
			int numberOfValuesInEachRow = imageOut->x;
			int offsetOfThePixel = (numberOfValuesInEachRow * centerY + centerX);
			imageOut->colors[colourType][offsetOfThePixel] = value;
		}
	}
	
	
	
	
	// Precompute the division
	compute_t rep = 1.0/ ((size*2 + 1) * (size*2 + 1));
	
	// Iterate over each pixel in the center region
	// We know the sum values for the line above
	// Use that knowledge to only add/subtract the corner values
	for(int centerY = size+1; centerY < (imageIn->y - size); centerY++) {
		
		// Find the sum of a single line above
		compute_t sumSubtractionLine = 0;
		compute_t sumAdditionLine = 0;
		for(int x = -size; x <= size; x++) {
			int currentX = size + x;
			int currentY = centerY-(size+1);
		
			// Now we can begin
			int numberOfValuesInEachRow = imageIn->x; 
			int offsetOfThePixel = (numberOfValuesInEachRow * currentY + currentX);
			sumSubtractionLine += imageIn->colors[colourType][offsetOfThePixel];
		}
		for(int x = -size; x <= size; x++) {
			int currentX = size + x;
			int currentY = centerY+size;
		
			// Now we can begin
			int numberOfValuesInEachRow = imageIn->x; 
			int offsetOfThePixel = (numberOfValuesInEachRow * currentY + currentX);
			sumAdditionLine += imageIn->colors[colourType][offsetOfThePixel];
		}

		
		// Now we move one pixel to the right, and compute the delta
		for(int centerX = size+1; centerX < (imageIn->x - size); centerX++) {
			// First we remove the top left value
			{
				int currentX = centerX - (size + 1);
				int currentY = centerY - (size + 1);
			
				int numberOfValuesInEachRow = imageIn->x; 
				int offsetOfThePixel = (numberOfValuesInEachRow * currentY + currentX);
				sumSubtractionLine -= imageIn->colors[colourType][offsetOfThePixel];
			}
			
			// Then we add the top right value
			{
				int currentX = centerX + size;
				int currentY = centerY - (size + 1);
			
				int numberOfValuesInEachRow = imageIn->x; 
				int offsetOfThePixel = (numberOfValuesInEachRow * currentY + currentX);
				sumSubtractionLine += imageIn->colors[colourType][offsetOfThePixel];
			}
			
			// Then we remove the bottom left value
			{
				int currentX = centerX - (size + 1);
				int currentY = centerY + size;
			
				int numberOfValuesInEachRow = imageIn->x; 
				int offsetOfThePixel = (numberOfValuesInEachRow * currentY + currentX);
				sumAdditionLine -= imageIn->colors[colourType][offsetOfThePixel];
			}
			
			// Then we add the top right value
			{
				int currentX = centerX + size;
				int currentY = centerY + size;
			
				int numberOfValuesInEachRow = imageIn->x; 
				int offsetOfThePixel = (numberOfValuesInEachRow * currentY + currentX);
				sumAdditionLine += imageIn->colors[colourType][offsetOfThePixel];
			}
			
			// The final sum is equal to the one above
			// 		* Minus the top line
			//		* Plus the bottom line 
			storage_t value = (lineBuffer[centerX] - sumSubtractionLine + sumAdditionLine) * rep;
			
			lineBuffer[centerX] = (lineBuffer[centerX] - sumSubtractionLine + sumAdditionLine);
			
			
			// Update the output image
			int numberOfValuesInEachRow = imageOut->x;
			int offsetOfThePixel = (numberOfValuesInEachRow * centerY + centerX);
			imageOut->colors[colourType][offsetOfThePixel] = value;
		}
	}
	
	// Free the buffer
	free(lineBuffer);
}

unsigned char clamp( storage_t value_org) {
	
	storage_t value = value_org;
	unsigned char val = (unsigned char) value;
	
	if (value < -1 )
		value = 257+value;
	if(value > 255)
		value = 255;
	
	
	static int count = 0;
	
	unsigned char val2 = (unsigned char) value;
	if(val != val2) {
	
		printf("val_org: %d, val2: %d, value_org: %f, value: %f\n", val, val2, value_org, value);
		count++;
		if(count == 100)
			exit(0);
	}
	
	return (unsigned char) value;
}

// Compute the final image
PPMImage * performNewIdeaFinalizationPlanes(AccurateImagePlanes *imageInSmall, AccurateImagePlanes *imageInLarge) {
	PPMImage *imageOut;
	imageOut = (PPMImage *)malloc(sizeof(PPMImage));
	imageOut->data = (PPMPixel*)malloc(imageInSmall->x * imageInSmall->y * sizeof(PPMPixel));
	
	imageOut->x = imageInSmall->x;
	imageOut->y = imageInSmall->y;
	
	/*int counDiffA[1024];
	for(int i = 0; i < 1024; i++) {
		counDiffA[i] = 0;
	}
	int *counDiff = &counDiffA[512];*/
	
	for(int i = 0; i < imageInSmall->x * imageInSmall->y; i++) {
		imageOut->data[i].red   = (unsigned char) (imageInLarge->colors[0][i] - imageInSmall->colors[0][i]);
		imageOut->data[i].green = (unsigned char) (imageInLarge->colors[1][i] - imageInSmall->colors[1][i]);
		imageOut->data[i].blue  = (unsigned char) (imageInLarge->colors[2][i] - imageInSmall->colors[2][i]);
		
		//counDiff[(int)(imageInLarge->colors[0][i] - imageInSmall->colors[0][i])]++;
		//counDiff[(int)(imageInLarge->colors[1][i] - imageInSmall->colors[1][i])]++;
		//counDiff[(int)(imageInLarge->colors[2][i] - imageInSmall->colors[2][i])]++;
	}
	
	/*for(int i = 0; i < 1024; i++) {
		if(counDiffA[i] != 0)
			printf("Deltas %d: %d\n", i-512, counDiffA[i]);
	}
	
	printf("\n\n");*/
	
	
	return imageOut;
}


int main(int argc, char *argv[]) {
	PPMImage *image;
	image = readPPM("flower.ppm");
	
	
	AccurateImagePlanes *imageAccurate1_tiny = convertImageToNewPlaneFormat(image);
	AccurateImagePlanes *imageAccurate2_tiny = convertImageToNewPlaneFormat(image);
	
	// Do each color channel
	for(int colour = 0; colour < 3; colour++) {
		int size = 1; // 2
		performNewIdeaIteration(imageAccurate2_tiny, imageAccurate1_tiny, colour, size);
		performNewIdeaIteration(imageAccurate1_tiny, imageAccurate2_tiny, colour, size);
		performNewIdeaIteration(imageAccurate2_tiny, imageAccurate1_tiny, colour, size);
		performNewIdeaIteration(imageAccurate1_tiny, imageAccurate2_tiny, colour, size);
		performNewIdeaIteration(imageAccurate2_tiny, imageAccurate1_tiny, colour, size);
	}
	
	
	AccurateImagePlanes *imageAccurate1_small = convertImageToNewPlaneFormat(image);
	AccurateImagePlanes *imageAccurate2_small = convertImageToNewPlaneFormat(image);
	
	// Do each color channel
	for(int colour = 0; colour < 3; colour++) {
		int size = 2; // 3
		performNewIdeaIteration(imageAccurate2_small, imageAccurate1_small, colour, size);
		performNewIdeaIteration(imageAccurate1_small, imageAccurate2_small, colour, size);
		performNewIdeaIteration(imageAccurate2_small, imageAccurate1_small, colour, size);
		performNewIdeaIteration(imageAccurate1_small, imageAccurate2_small, colour, size);
		performNewIdeaIteration(imageAccurate2_small, imageAccurate1_small, colour, size);
	}
	
	AccurateImagePlanes *imageAccurate1_medium = convertImageToNewPlaneFormat(image);
	AccurateImagePlanes *imageAccurate2_medium = convertImageToNewPlaneFormat(image);
	
	// Do each color channel
	for(int colour = 0; colour < 3; colour++) {
		int size = 3; // 5
		performNewIdeaIteration(imageAccurate2_medium, imageAccurate1_medium, colour, size);
		performNewIdeaIteration(imageAccurate1_medium, imageAccurate2_medium, colour, size);
		performNewIdeaIteration(imageAccurate2_medium, imageAccurate1_medium, colour, size);
		performNewIdeaIteration(imageAccurate1_medium, imageAccurate2_medium, colour, size);
		performNewIdeaIteration(imageAccurate2_medium, imageAccurate1_medium, colour, size);
	}
	
	AccurateImagePlanes *imageAccurate1_large = convertImageToNewPlaneFormat(image);
	AccurateImagePlanes *imageAccurate2_large = convertImageToNewPlaneFormat(image);
	
	// Do each color channel
	for(int colour = 0; colour < 3; colour++) {
		performNewIdeaIteration(imageAccurate2_large, imageAccurate1_large, colour, 8);
		performNewIdeaIteration(imageAccurate1_large, imageAccurate2_large, colour, 8);
		performNewIdeaIteration(imageAccurate2_large, imageAccurate1_large, colour, 8);
		performNewIdeaIteration(imageAccurate1_large, imageAccurate2_large, colour, 8);
		performNewIdeaIteration(imageAccurate2_large, imageAccurate1_large, colour, 8);
	}
	
	PPMImage *final_tiny = performNewIdeaFinalizationPlanes(imageAccurate2_tiny,  imageAccurate2_small);//convertNewFormatToPPM(imageAccurate2_small);
	writePPM("flower_tiny_c.ppm",final_tiny);
	
	PPMImage *final_small = performNewIdeaFinalizationPlanes(imageAccurate2_small,  imageAccurate2_medium);//convertNewFormatToPPM(imageAccurate2_small);
	writePPM("flower_small_c.ppm",final_small);
	
	PPMImage *final_medium = performNewIdeaFinalizationPlanes(imageAccurate2_medium,  imageAccurate2_large);//convertNewFormatToPPM(imageAccurate2_small);
	writePPM("flower_medium.ppm",final_medium);
	
	
	imageAccurate1_small = convertImageToNewPlaneFormat(final_tiny);
	imageAccurate2_small = convertImageToNewPlaneFormat(final_tiny);
	
	
	// Do each color channel
	for(int colour = 0; colour < 3; colour++) {
		int size = 5;
		performNewIdeaIteration(imageAccurate2_small, imageAccurate1_small, colour, size);
		performNewIdeaIteration(imageAccurate1_small, imageAccurate2_small, colour, size);
		performNewIdeaIteration(imageAccurate2_small, imageAccurate1_small, colour, size);
		performNewIdeaIteration(imageAccurate1_small, imageAccurate2_small, colour, size);
		performNewIdeaIteration(imageAccurate2_small, imageAccurate1_small, colour, size);
	}
	
	imageAccurate1_medium = convertImageToNewPlaneFormat(final_tiny);
	imageAccurate2_medium = convertImageToNewPlaneFormat(final_tiny);
	
	// Do each color channel
	for(int colour = 0; colour < 3; colour++) {
		int size = 8;
		performNewIdeaIteration(imageAccurate2_medium, imageAccurate1_medium, colour, size);
		performNewIdeaIteration(imageAccurate1_medium, imageAccurate2_medium, colour, size);
		performNewIdeaIteration(imageAccurate2_medium, imageAccurate1_medium, colour, size);
		performNewIdeaIteration(imageAccurate1_medium, imageAccurate2_medium, colour, size);
		performNewIdeaIteration(imageAccurate2_medium, imageAccurate1_medium, colour, size);
	}
	
	PPMImage *final_small_2 = performNewIdeaFinalizationPlanes(imageAccurate2_small,  imageAccurate2_medium);//convertNewFormatToPPM(imageAccurate2_small);
	writePPM("flower_small_2.ppm",final_small_2);
	
	
}
