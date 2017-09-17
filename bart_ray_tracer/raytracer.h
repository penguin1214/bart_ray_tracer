//
// Created by yang on 17-8-3.
//

#ifndef BART_RAY_TRACER_RAYTRACER_H
#define BART_RAY_TRACER_RAYTRACER_H

#include <stack>
#include <string>
#include <fstream>
#include <iostream>
#include "scene.h"
#include "transform.h"
#include "vec3f.h"
#include "ray.h"

class RayTracer {
public:
	int nsample;
	char* filename;
	Scene* scene;
	Camera* camera;

	float start_time;
	float end_time;
	float nframes;

	RayTracer(char* f, int d) : filename(f) {
		nsample = 5;
		scene = new Scene();
		camera = new Camera();
		transformHierarchy.push(NULL);
		scene->max_depth = d;
	}

	void setTime(float s, float e) {
		start_time = s; end_time = e;
	}

	void setFrame(int n) { nframes = n; }

	void render() {
		std::cout << "rendering... " << std::endl;
		std::ofstream myfile;
		myfile.open("image.ppm");

		myfile << "P3\n" << camera->film->width << " " << camera->film->height << "\n255\n";

		//for (int i = camera->film->height - 1; i >= 0; --i) {
		for (int j = camera->film->height; j > 0; --j) {
			double count = (1.0 - double(j) / double(camera->film->height)) * 100.0;
			std::cout << count << "% done" << std::endl;
			for (int i = 0; i < camera->film->width; ++i) {
				vec3f col(0, 0, 0);
				for (int s = 0; s < nsample; ++s) {
					float random = ((double) rand() / (RAND_MAX)) + 1;
					float u = float(i + random) / float(camera->film->width);  // u, v cord??
					float v = float(j + random) / float(camera->film->height);
					Ray r = camera->get_ray(u, v);
					// trace
					if (scene->max_depth == 0) { col += vec3f(0.0); }
					col += scene->trace(r, 0);    // 110561 shapes
				}
				col /= float(nsample);
				// gamma correct
				col = vec3f(sqrt(col.e[0]), sqrt(col.e[1]), sqrt(col.e[2]));
				int ir = int(255.99 * col.e[0]);  // why use 255.99?
				int ig = int(255.99 * col.e[1]);
				int ib = int(255.99 * col.e[2]);
				myfile << ir << " " << ig << " " << ib << "\n";
				// std::cout << ir << " " << ig << " " << ib << std::endl;
			}
		}
		myfile.close();
	}

//    vec3f trace(const Ray& r, std::vector<Shape* > shapes) {
//        for (std::vector<Shape* >::iterator it = shapes.begin(); it != shapes.end(); ++it) {
//            HitRecord record;
//            if (depth == 0) {
//                return vec3f(0.0);
//            }
//
//        }
//    }

	/* TODO: */
	void savePPM() {}
};

// !!! ATTENTION
extern RayTracer* rayTracer;

#endif //BART_RAY_TRACER_RAYTRACER_H
