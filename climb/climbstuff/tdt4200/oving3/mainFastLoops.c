#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "ppm.h"

// Image from:
// http://7-themes.com/6971875-funny-flowers-pictures.html

typedef float storage_t;
typedef float compute_t;

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
	for(int centerY = 0; centerY < size; centerY++) {
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
	for(int centerY = size; centerY < (imageIn->y-size); centerY++) {
		for(int centerX = 0; centerX < size; centerX++) {
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
	for(int centerY = size; centerY < (imageIn->y-size); centerY++) {
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
	
	
	
	// Iterate over each pixel in the center region
	// Precompute the division
	compute_t rep = 1.0/ ((size*2 + 1) * (size*2 + 1));
	for(int centerY = size; centerY < (imageIn->y - size); centerY++) {
		for(int centerX = size; centerX < (imageIn->x - size); centerX++) {
			// For each pixel we compute the magic number
			compute_t sum = 0;
			//int countIncluded = 0;
			for(int y = -size; y <= size; y++) {
				for(int x = -size; x <= size; x++) {
					int currentX = centerX + x;
					int currentY = centerY + y;
					
					// Now we can begin
					int numberOfValuesInEachRow = imageIn->x; 
					int offsetOfThePixel = (numberOfValuesInEachRow * currentY + currentX);
					sum += imageIn->colors[colourType][offsetOfThePixel];
				
				}
			}
			
			// Now we compute the final value
			storage_t value = (storage_t) (sum * rep);
			
	
			// Update the output image
			int numberOfValuesInEachRow = imageOut->x;
			int offsetOfThePixel = (numberOfValuesInEachRow * centerY + centerX);
			imageOut->colors[colourType][offsetOfThePixel] = value;
		}
	}
}

// Compute the final image
PPMImage * performNewIdeaFinalizationPlanes(AccurateImagePlanes *imageInSmall, AccurateImagePlanes *imageInLarge) {
	PPMImage *imageOut;
	imageOut = (PPMImage *)malloc(sizeof(PPMImage));
	imageOut->data = (PPMPixel*)malloc(imageInSmall->x * imageInSmall->y * sizeof(PPMPixel));
	
	imageOut->x = imageInSmall->x;
	imageOut->y = imageInSmall->y;
	
	for(int i = 0; i < imageInSmall->x * imageInSmall->y; i++) {
		imageOut->data[i].red   = (unsigned char) (imageInLarge->colors[0][i] - imageInSmall->colors[0][i]);
		imageOut->data[i].green = (unsigned char) (imageInLarge->colors[1][i] - imageInSmall->colors[1][i]);
		imageOut->data[i].blue  = (unsigned char) (imageInLarge->colors[2][i] - imageInSmall->colors[2][i]);
	}
	
	
	return imageOut;
}


int main(int argc, char *argv[]) {
	PPMImage *image;
	image = readPPM("flower.ppm");
	
	
	AccurateImagePlanes *imageAccurate1_tiny = convertImageToNewPlaneFormat(image);
	AccurateImagePlanes *imageAccurate2_tiny = convertImageToNewPlaneFormat(image);
	
	// Do each color channel
	for(int colour = 0; colour < 3; colour++) {
		int size = 2; // 2
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
		int size = 3; // 3
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
		int size = 5; // 5
		performNewIdeaIteration(imageAccurate2_medium, imageAccurate1_medium, colour, size);
		performNewIdeaIteration(imageAccurate1_medium, imageAccurate2_medium, colour, size);
		performNewIdeaIteration(imageAccurate2_medium, imageAccurate1_medium, colour, size);
		performNewIdeaIteration(imageAccurate1_medium, imageAccurate2_medium, colour, size);
		performNewIdeaIteration(imageAccurate2_medium, imageAccurate1_medium, colour, size);
	}
	
	/*AccurateImage *imageAccurate1_large = convertImageToNewFormat(image);
	AccurateImage *imageAccurate2_large = convertImageToNewFormat(image);
	
	// Do each color channel
	for(int colour = 0; colour < 3; colour++) {
		performNewIdeaIteration(imageAccurate2_large, imageAccurate1_large, colour, 8);
		performNewIdeaIteration(imageAccurate1_large, imageAccurate2_large, colour, 8);
		performNewIdeaIteration(imageAccurate2_large, imageAccurate1_large, colour, 8);
		performNewIdeaIteration(imageAccurate1_large, imageAccurate2_large, colour, 8);
		performNewIdeaIteration(imageAccurate2_large, imageAccurate1_large, colour, 8);
	}*/
	
	PPMImage *final_tiny = performNewIdeaFinalizationPlanes(imageAccurate2_tiny,  imageAccurate2_small);//convertNewFormatToPPM(imageAccurate2_small);
	writePPM("flower_tiny_c.ppm",final_tiny);
	
	PPMImage *final_small = performNewIdeaFinalizationPlanes(imageAccurate2_small,  imageAccurate2_medium);//convertNewFormatToPPM(imageAccurate2_small);
	writePPM("flower_small_c.ppm",final_small);
	
	//PPMImage *final_medium = performNewIdeaFinalization(imageAccurate2_medium,  imageAccurate2_large);//convertNewFormatToPPM(imageAccurate2_small);
	//writePPM("flower_medium.ppm",final_medium);
	
	/*
	imageAccurate1_small = convertImageToNewFormat(final_tiny);
	imageAccurate2_small = convertImageToNewFormat(final_tiny);
	
	
	// Do each color channel
	for(int colour = 0; colour < 3; colour++) {
		int size = 5;
		performNewIdeaIteration(imageAccurate2_small, imageAccurate1_small, colour, size);
		performNewIdeaIteration(imageAccurate1_small, imageAccurate2_small, colour, size);
		performNewIdeaIteration(imageAccurate2_small, imageAccurate1_small, colour, size);
		performNewIdeaIteration(imageAccurate1_small, imageAccurate2_small, colour, size);
		performNewIdeaIteration(imageAccurate2_small, imageAccurate1_small, colour, size);
	}
	
	imageAccurate1_medium = convertImageToNewFormat(final_tiny);
	imageAccurate2_medium = convertImageToNewFormat(final_tiny);
	
	// Do each color channel
	for(int colour = 0; colour < 3; colour++) {
		int size = 8;
		performNewIdeaIteration(imageAccurate2_medium, imageAccurate1_medium, colour, size);
		performNewIdeaIteration(imageAccurate1_medium, imageAccurate2_medium, colour, size);
		performNewIdeaIteration(imageAccurate2_medium, imageAccurate1_medium, colour, size);
		performNewIdeaIteration(imageAccurate1_medium, imageAccurate2_medium, colour, size);
		performNewIdeaIteration(imageAccurate2_medium, imageAccurate1_medium, colour, size);
	}
	
	PPMImage *final_small_2 = performNewIdeaFinalization(imageAccurate2_small,  imageAccurate2_medium);//convertNewFormatToPPM(imageAccurate2_small);
	writePPM("flower_small_2.ppm",final_small_2);
	*/
	
}
