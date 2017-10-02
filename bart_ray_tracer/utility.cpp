#ifndef UTILITY_H
#include "utility.h"
#include <fstream>
#include <iostream>
#include <algorithm>

#include "common.h"

float deg2rad(float degree) {
	return degree*M_PI/180;
}

void writePPMBin(float *image, int size_x, int size_y, char *filename) {
	std::ofstream ofs(filename, std::ios::out | std::ios::binary);
	ofs << "P6\n" << size_x << " " << size_y << "\n255\n";
	for (int j = 0; j < size_y; j++)  // loop over pixels, write RGB values
		for (int i = 0; i < size_x; i++) {
			int index = j*size_x + i;
			/*if (image[index * 3 + 0] || image[index * 3 + 1] || image[index * 3 + 2]) {
				std::cout << image[index * 3 + 0] * 255 << std::endl;
				std::cout << image[index * 3 + 1] * 255 << std::endl;
				std::cout << image[index * 3 + 2] * 255 << std::endl;
				std::cout << "===================" << std::endl;
			}*/
			ofs << (unsigned char)((std::min)(1.0f, image[index * 3 + 0]) * 255);
			ofs << (unsigned char)((std::min)(1.0f, image[index * 3 + 1]) * 255);
			ofs << (unsigned char)((std::min)(1.0f, image[index * 3 + 2]) * 255);

		}
	ofs.flush();
	printf("Saved image \n");
}

void writePPMDec(float *image) {}

void colorScale(float *image, int npixel) {
	float max_r = 0.0f, max_g = 0.0f, max_b = 0.0f;
	float min_r = 0.0f, min_g = 0.0f, min_b = 0.0f;
	for (int i = 0; i < npixel; i++) {
		// red
		if (min_r > image[i + 0]) min_r = image[i + 0];
		if (max_r < image[i + 0]) max_r = image[i + 0];
		// green
		if (min_g > image[i + 1]) min_g = image[i + 1];
		if (max_g > image[i + 1]) max_g = image[i + 1];
		// blue
		if (min_b > image[i + 2]) min_b = image[i + 2];
		if (max_b > image[i + 2]) max_b = image[i + 2];
	}
}

#endif 
