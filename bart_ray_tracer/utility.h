//
// Created by Jinglei Yang on 8/8/17.
//

#ifndef BART_RAY_TRACER_UTILITY_H
#define BART_RAY_TRACER_UTILITY_H

float deg2rad(float degree);

void writePPMBin(float *image, int size_x, int size_y, char *filename);
void writePPMDec(float *image);

void colorScale(float *image, int npixel);

#endif //BART_RAY_TRACER_UTILITY_H
