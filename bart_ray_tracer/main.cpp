#include <iostream>TransformHierarchy *transform
#include "raytracer.h"
#include "parse.h"

#define MAX_DEPTH 10

RayTracer* rayTracer;
std::stack<TransformHierarchy* >transformHierarchy;

int main(int argc, char** argv) {
	/*if (argc!=2) {
		std::cerr << argv[0] << " <file_name>." << std::endl;
		return -1;
	}*/
	std::string str_filename = "kitchen/test.aff";
	char filename[1024];
	strncpy(filename, str_filename.c_str(), sizeof(filename));
	filename[sizeof(filename) - 1] = 0;
	FILE *fp = fopen(filename, "r");
	//FILE *fp = fopen(argv[1], "r");
	if (!fp) {
		std::cout << "Open file failed." << std::endl;
		return 1;
	}


	// rayTracer declared in raytracer.h
	//rayTracer = new RayTracer(argv[1], MAX_DEPTH);
	rayTracer = new RayTracer(filename, MAX_DEPTH);

	/*
	 * four keywords are related to shape:
	 * 'm': triangle mesh
	 * 'p': polygon, is converted to triangle mesh when parsing
	 * 's': sphere
	 * 'c': cone or cylinder primitive
	 */
	if (!viParseFile(fp)) {
		std::cout << "Parse file failed." << std::endl;
	}

	rayTracer->render();

	int t;
	std::cin >> t;
	return 0;
}