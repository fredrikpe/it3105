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


// Convert a PPM image to a high-precision format
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

PPMImage * performNewIdeaFinalization(AccurateImage *imageInSmall, AccurateImage *imageInLarge) {
	PPMImage *imageOut;
	imageOut = (PPMImage *)malloc(sizeof(PPMImage));
	imageOut->data = (PPMPixel*)malloc(imageInSmall->x * imageInSmall->y * sizeof(PPMPixel));
	
	imageOut->x = imageInSmall->x;
	imageOut->y = imageInSmall->y;
	
	int counDiff[256];
	for(int i = 0; i < 256; i++) {
		counDiff[i] = 0;
	}
	
	for(int i = 0; i < imageInSmall->x * imageInSmall->y; i++) {
		imageOut->data[i].red   = (unsigned char) fabs(imageInLarge->data[i].red - imageInSmall->data[i].red);
		imageOut->data[i].green = (unsigned char) fabs(imageInLarge->data[i].green - imageInSmall->data[i].green);
		imageOut->data[i].blue  = (unsigned char) fabs(imageInLarge->data[i].blue - imageInSmall->data[i].blue);
		
		counDiff[imageOut->data[i].red]++;
		counDiff[imageOut->data[i].green]++;
		counDiff[imageOut->data[i].blue]++;
		
		/*if(imageOut->data[i].red > 0) counDiff0++;
		if(imageOut->data[i].green > 0) counDiff0++;
		if(imageOut->data[i].blue > 0) counDiff0++;
		
		if(imageOut->data[i].red > 1) counDiff1++;
		if(imageOut->data[i].green > 1) counDiff1++;
		if(imageOut->data[i].blue > 1) counDiff1++;
		
		if(imageOut->data[i].red > 5) counDiff1++;
		if(imageOut->data[i].green > 5) counDiff1++;
		if(imageOut->data[i].blue > 5) counDiff1++;*/
	}
	
	//printf("Deltas 0: %d\n", counDiff0);
	//printf("Deltas 1: %d\n", counDiff1);
	
	for(int i = 0; i < 256; i++) {
		if(counDiff[i] != 0)
			printf("Deltas %d: %d\n", i, counDiff[i]);
	}
	
	return imageOut;
}


int main(int argc, char *argv[]) {
	PPMImage *image;
	
	image = readPPM("flower_tiny.ppm");
	AccurateImage *imageAccurate1_tiny_org = convertImageToNewFormat(image);
	
	image = readPPM("flower_tiny_c.ppm");
	AccurateImage *imageAccurate2_tiny_c = convertImageToNewFormat(image);


	
	PPMImage *final_tiny = performNewIdeaFinalization(imageAccurate1_tiny_org,  imageAccurate2_tiny_c);//convertNewFormatToPPM(imageAccurate2_small);
	writePPM("flower_tiny_delta.ppm",final_tiny);
	
	//PPMImage *final_small = performNewIdeaFinalization(imageAccurate2_small,  imageAccurate2_medium);//convertNewFormatToPPM(imageAccurate2_small);
	//writePPM("flower_small_delta.ppm",final_small);
	

	
}
