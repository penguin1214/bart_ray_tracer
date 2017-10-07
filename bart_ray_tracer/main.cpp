/*
 * BART ray tracer uses **single-faced** triangles!
 */
#include <iostream>
#include "raytracer.h"
#include "parse.h"

#define MAX_DEPTH 3

RayTracer* rayTracer;
std::stack<TransformHierarchy* >transformHierarchy;

#include "scene.h"
#include <math.h>
#include <algorithm>


int main() {
	vec3f n(0.0, 1.0, 0.0);
	vec3f v = unit(vec3f(1.0, 1.75, 0.0));
	float eta = 0.75;	// 1.33 / 1.0
	vec3f ref;
	if (refract(v, n, eta, ref)) {
		std::cout << ref << std::endl;
		std::cout << abs(ref.y()) / abs(ref.x()) << std::endl;
	}
	int c;
	std::cin >> c;
}
//int main(int argc, char** argv) {
//
//	vec3f n(0.0, 1.0, 0.0);
//	vec3f v = unit(vec3f(1.0, -1.0, 0.0));
//	float eta = 1.33;	// 1.33 / 1.0
//	vec3f ref;
//	if (refract(v, n, eta, ref)) {
//		std::cout << ref << std::endl;
//	}
//
//	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
//	/*if (argc!=2) {
//		std::cerr << argv[0] << " <file_name>." << std::endl;
//		return -1;
//	}*/
//	std::string str_filename = "test.aff";
//	char filename[1024];
//	strncpy(filename, str_filename.c_str(), sizeof(filename));
//	filename[sizeof(filename) - 1] = 0;
//	FILE *fp = fopen(filename, "r");
//	//FILE *fp = fopen(argv[1], "r");
//	if (!fp) {
//		std::cout << "Open file failed." << std::endl;
//		return 1;
//	}
//
//
//	// rayTracer declared in raytracer.h
//	//rayTracer = new RayTracer(argv[1], MAX_DEPTH);
//	rayTracer = new RayTracer(filename);
//
//	/*
//	 * four keywords are related to shape:
//	 * 'm': triangle mesh
//	 * 'p': polygon, is converted to triangle mesh when parsing
//	 * 's': sphere
//	 * 'c': cone or cylinder primitive
//	 */
//	if (!viParseFile(fp)) {
//		std::cout << "Parse file failed." << std::endl;
//	}
//
//	float *image = new float[3 * rayTracer->scene->camera->film->height*rayTracer->scene->camera->film->width];
//	char *ofname = "image.ppm";
//
//	rayTracer->init();
//	// rayTracer->render(image);
//	// do scale
//	colorScale(image, rayTracer->scene->camera->film->height*rayTracer->scene->camera->film->width);
//	writePPMBin(image, rayTracer->scene->camera->film->width, rayTracer->scene->camera->film->height, ofname);
//
//	int tm;
//	std::cin >> tm;
//	return 0;
//}