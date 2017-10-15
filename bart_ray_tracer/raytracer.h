//
// Created by yang on 17-8-3.
//

#ifndef BART_RAY_TRACER_RAYTRACER_H
#define BART_RAY_TRACER_RAYTRACER_H


#include "core.h"
#include "scene.h"
#include "transform.h"
#include "ray.h"

class RayTracer {
public:
	int nsample;
	char* filename;
	Scene* scene;
	float start_time;
	float end_time;
	float nframes;

	RayTracer(char* f);

	void init();
	void setTime(float s, float e);
	void setFrame(int n);
	void renderFrame(float time, float *image);
	void render();
};

// !!! ATTENTION
extern RayTracer* rayTracer;

#endif //BART_RAY_TRACER_RAYTRACER_H
