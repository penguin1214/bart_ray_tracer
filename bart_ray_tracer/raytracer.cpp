#include "raytracer.h"

#include <string>
#include <fstream>
#include <omp.h>
#include <iostream>
#include <stack>

RayTracer::RayTracer(char* f) : filename(f) {
	nsample = 1;
	scene = new Scene();
	transformHierarchy.push(NULL);
}

void RayTracer::init() {
	// scene->bvh = new BVH(scene->meshes);
}

void RayTracer::setTime(float s, float e) {
	start_time = s; end_time = e;
}

void RayTracer::setFrame(int n) { nframes = n; }

void RayTracer::renderFrame(float time, float *image) {
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

void RayTracer::render() {
	// build BVH
	// scene->bvh->_octree->build();	// update tree
	float time_step = (end_time - start_time) / nframes;

	std::cout << "rendering..." << std::endl;
	// for every frame
	for (int i = 0; i < nframes; i++) {
		std::cout << "frame " << i << std::endl;
		float time = start_time + i * time_step;
		float *image = new float[3 * rayTracer->scene->camera->film->height*rayTracer->scene->camera->film->width];
		renderFrame(time, image);

		// do scale
		colorScale(image, rayTracer->scene->camera->film->height*rayTracer->scene->camera->film->width);

		std::string s = (std::to_string(i) + ".ppm");
		const char *ofname = s.c_str();
		writePPMBin(image, rayTracer->scene->camera->film->width, rayTracer->scene->camera->film->height, ofname);
	}

}