#include "test.h"

void test_triangle_intersection() {
	vec3f v0(0.0);
	vec3f v1(1.0, 0.0, 0.0);
	vec3f v2(0.0, 1.0, 0.0);
	Ray r(vec3f(0.0, 0.0, 1.0), vec3f(0.0, 0.0, -1.0));

	if (Triangle::intersect(r, v0, v1, v2)) std::cout << "intersect" << std::endl;
	else std::cout << "no intersection" << std::endl;
}