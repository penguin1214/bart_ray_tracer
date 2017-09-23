#ifndef UTILITY_H
#include "utility.h"
#include <fstream>
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
			ofs << (unsigned char)((std::min)(1.0f, image[index * 3 + 0]) * 255);
			ofs << (unsigned char)((std::min)(1.0f, image[index * 3 + 1]) * 255);
			ofs << (unsigned char)((std::min)(1.0f, image[index * 3 + 2]) * 255);

		}
	ofs.flush();
	printf("Saved image \n");
}

void writePPMDec(float *image) {

}
#endif 
