//
// Created by yang on 17-8-3.
//

#ifndef BART_RAY_TRACER_RAYTRACER_H
#define BART_RAY_TRACER_RAYTRACER_H

#include <stack>
#include <string>
#include <fstream>
#include <iostream>
#include <omp.h>

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

	RayTracer(char* f) : filename(f) {
		nsample = 1;
		scene = new Scene();
		transformHierarchy.push(NULL);
	}

	void setTime(float s, float e) {
		start_time = s; end_time = e;
	}

	void setFrame(int n) { nframes = n; }

	void render(float* image) {
		// build BVH

		std::cout << "rendering..." << std::endl;
		for (int j = 0; j < scene->camera->film->height; ++j) {
			double count = double(j) / double(scene->camera->film->height) * 100.0;

#pragma omp parallel for
			std::cout << count << "% done" << std::endl;

			for (int i = 0; i < scene->camera->film->width; ++i) {
				int index = j*scene->camera->film->width + i;

				vec3f col(0, 0, 0);
				for (int s = 0; s < nsample; ++s) {
					float random = ((double)rand() / (RAND_MAX)) + 1;
					float u = float(i + random) / float(scene->camera->film->width);  // u, v cord??
					float v = float(j + random) / float(scene->camera->film->height);
					Ray r = scene->camera->get_ray(u, v);
					// trace
					col += scene->trace(r, 0, 1.0);    // vacuum ior is 1.0
				}
				col /= float(nsample);
				// gamma correct
				col = vec3f(sqrt(col.e[0]), sqrt(col.e[1]), sqrt(col.e[2]));

				image[index * 3 + 0] = col.e[0];
				image[index * 3 + 1] = col.e[1];
				image[index * 3 + 2] = col.e[2];
			}
		}
	}
};

// !!! ATTENTION
extern RayTracer* rayTracer;

#endif //BART_RAY_TRACER_RAYTRACER_H
