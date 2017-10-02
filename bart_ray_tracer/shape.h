//
// Created by yang on 17-8-3.
//

#ifndef BART_RAY_TRACER_SHAPE_H
#define BART_RAY_TRACER_SHAPE_H

#include <math.h>

#include "core.h"
class Ray;
class Material;
class Mesh;
class Shape;

struct HitRecord {
	float t;
	vec3f p;
	vec3f norm;
	Shape* obj;
};

struct ExtentHitRecord {
	float t_near = 0;
	float t_far = INFINITY;
	int plane_idx;
};

class Shape {
public:
	Mesh *mesh_ptr;

	Shape();

	virtual bool intersect(Ray& r, HitRecord& rec);
	virtual Material* material();
};

class BBox : public Shape {
public:
	BBox();
	BBox(vec3f &min, vec3f &max) : p_min(min), p_max(max) {}
	~BBox();

	bool intersect();	// TODO

	vec3f p_min;
	vec3f p_max;
};

class Triangle : public Shape {
public:
	unsigned int vertexIndex[3];
	unsigned int normalIndex[3];
	unsigned int txtIndex[3];

	Triangle();
	Triangle(Mesh* m);

	bool intersect(Ray& r, HitRecord& rec);
	Material* material();
	void setVertexIndex(unsigned int i0, unsigned int i1, unsigned int i2);
	void setNormalIndex(unsigned int i0, unsigned int i1, unsigned int i2);
	void setTxtIndex(unsigned int i0, unsigned int i1, unsigned int i2);

	static void barycentric(vec3f P, vec3f A, vec3f B, vec3f C, float &baryA, float &baryB, float &baryC);
};

class Sphere : public Shape {
public:
	vec3f center;
	float radius;

	Sphere() {}
	Sphere(vec3f c, float r) : center(c), radius(r) {}

	inline bool intersect(const Ray& r, HitRecord &rec);
};

class Cone : public Shape {
public:

};

class Plane : public Shape {
public:
	vec3f norm;
	float d;

	Plane() {}
	Plane(vec3f n, float dis) : norm(n), d(dis) {}

	//    inline bool intersect(const Ray& r, HitRecord &rec);
};

#endif //BART_RAY_TRACER_SHAPE_H
