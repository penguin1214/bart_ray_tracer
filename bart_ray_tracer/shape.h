//
// Created by yang on 17-8-3.
//

#ifndef BART_RAY_TRACER_SHAPE_H
#define BART_RAY_TRACER_SHAPE_H

#include "vec3f.h"
#include "ray.h"
#include "material.h"
#include "mesh.h"

struct HitRecord;   // forward declaration

class Shape {
public:
	Mesh *mesh_ptr;

	Shape() { mesh_ptr = nullptr; }

	virtual bool intersect(Ray& r, HitRecord& rec) {
		std::cout << "Child intersect() is not implement." << std::endl;
		return false;
	}

	virtual Material material() {
		std::cout << "Child material() is not implement." << std::endl;
		return Material();
	}
};

struct HitRecord {
	float t;
	vec3f p;    // point coord
	vec3f norm;
	Shape* obj;
};

class Triangle: public Shape {
public:
	unsigned int vertexIndex[3];
	unsigned int normalIndex[3];
	unsigned int txtIndex[3];

	Triangle() {
		for (int i = 0; i < 3; ++i) {
			vertexIndex[i] = -1;
			normalIndex[i] = -1;
			txtIndex[i] = -1;
		}
	}

	Triangle(Mesh* m) {
		for (int i = 0; i < 3; ++i) {
			vertexIndex[i] = -1;
			normalIndex[i] = -1;
			txtIndex[i] = -1;
		}
		mesh_ptr = m;
	}

	inline bool intersect(Ray& r, HitRecord& rec);

	inline Material material() { return mesh_ptr->material; }

	void setVertexIndex(unsigned int i0, unsigned int i1, unsigned int i2) {
		vertexIndex[0] = i2; vertexIndex[1] = i1; vertexIndex[2] = i0;
	}

	void setNormalIndex(unsigned int i0, unsigned int i1, unsigned int i2) {
		normalIndex[0] = i2; normalIndex[1] = i1; normalIndex[2] = i0;
	}

	void setTxtIndex(unsigned int i0, unsigned int i1, unsigned int i2) {
		txtIndex[0] = i2; txtIndex[1] = i1; txtIndex[2] = i0;
	}

	static inline void barycentric(vec3f P, vec3f A, vec3f B, vec3f C, float &baryA, float &baryB, float &baryC) {
		vec3f v0 = B - A, v1 = C - A, v2 = P - A;
		float d00 = dot(v0, v0);
		float d01 = dot(v0, v1);
		float d11 = dot(v1, v1);
		float d20 = dot(v2, v0);
		float d21 = dot(v2, v1);
		float denom = d00 * d11 - d01 * d01;

		baryA = (d11 * d20 - d01 * d21) / denom;
		baryB = (d00 * d21 - d01 * d20) / denom;
		baryC = 1.0 - baryA - baryB;

		return;
	}
};

class Sphere: public Shape {
public:
	vec3f center;
	float radius;

	Sphere() {}
	Sphere(vec3f c, float r) : center(c), radius(r) {}

	inline bool intersect(const Ray& r, HitRecord &rec);
};

class Cone: public Shape {
public:

};

class Plane: public Shape {
public:
	vec3f norm;
	float d;

	Plane() {}
	Plane(vec3f n, float dis) : norm(n), d(dis) {}

//    inline bool intersect(const Ray& r, HitRecord &rec);
};

inline bool Triangle::intersect(Ray& r, HitRecord& rec) {
	/* TODO: Moller-Trumbore */
	vec3f* verts = mesh_ptr->_verts_world;	// use world(transformed) coordinates!
	float eps = 1e-4;   // less than 1e-4, then

	// counter-clockwise
	vec3f v0 = verts[vertexIndex[0]];
	vec3f v1 = verts[vertexIndex[1]];
	vec3f v2 = verts[vertexIndex[2]];


	// compute plane normal
	vec3f n = cross(v1-v0, v2-v0);
	float d = dot(v0, n);

	// parallel
	if (std::abs(dot(n, r.direction())) < eps) return false;

	float t = - (dot(n, r.origin())+d) / dot(n, r.direction());
	if (t<0) return false;  // behind

	// check if in triangle
	vec3f p = r.origin() + t*r.direction();
	if (dot(n, cross((v1-v0),(p-v0))) < 0)
		return false;
	if (dot(n, cross((v2-v1),(p-v1))) < 0)
		return false;
	if (dot(n, cross((v0-v2),(p-v2))) < 0)
		return false;

	rec.t = t;
	rec.p = p;
	rec.obj = this;

	// special treatment for normals!
	if (mesh_ptr->_normals == NULL || mesh_ptr->nnorms == 0) { // no read-in normals
		rec.norm = unit(n);
	}
	else {
		// normal intersection
		// TODO
		// use average instead now
		n = (mesh_ptr->_normals_world[normalIndex[0]]
			+ mesh_ptr->_normals_world[normalIndex[1]]
			+ mesh_ptr->_normals_world[normalIndex[2]]) / 3;
	}

	std::cout << v0 << std::endl << v1 << std::endl << v2 << std::endl;
	std::cout << "intersect!" << std::endl;
	std::cout << "============================" << std::endl;
	return true;
}

inline bool Sphere::intersect(const Ray& r, HitRecord& rec) {
	vec3f oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float disc = b*b-a*c;
	if ((sqrt(disc)) >= 0) {
		// calculate hit point
		float temp = (-b - sqrt(b * b - a * c)) / a;
		rec.t = temp;
		rec.p = r.point_at_parameter(temp);
		rec.norm = (rec.p - center) / radius; // get unit normal
		rec.obj = this;
		std::cout << "sphere intersect!" << std::endl;
		return true;
	}
	std::cout << "sphere miss" << std::endl;
	return false;
}

#endif //BART_RAY_TRACER_SHAPE_H
