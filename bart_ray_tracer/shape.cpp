#include "shape.h"
#include "material.h"
#include "mesh.h"
#include "ray.h"

//////////////////////////////////////////////////////////////////////////
/// class Shape
//////////////////////////////////////////////////////////////////////////
Shape::Shape() { mesh_ptr = nullptr; }

bool Shape::intersect(Ray& r, HitRecord& rec) {
	std::cout << "Child intersect() is not implement." << std::endl;
	return false;
}

Material* Shape::material() {
	std::cout << "Child material() is not implement." << std::endl;
	return new Material();
}

//////////////////////////////////////////////////////////////////////////
/// class BBox
//////////////////////////////////////////////////////////////////////////
BBox::BBox() {}
BBox::~BBox() {}
bool BBox::intersect() { return false; }

//////////////////////////////////////////////////////////////////////////
/// class Triangle
//////////////////////////////////////////////////////////////////////////
Triangle::Triangle() {
	for (int i = 0; i < 3; ++i) {
		vertexIndex[i] = -1;
		normalIndex[i] = -1;
		txtIndex[i] = -1;
	}
}

Triangle::Triangle(Mesh* m) {
	for (int i = 0; i < 3; ++i) {
		vertexIndex[i] = -1;
		normalIndex[i] = -1;
		txtIndex[i] = -1;
	}
	mesh_ptr = m;
}

bool Triangle::intersect(Ray& r, HitRecord& rec) {
	vec3f* verts = mesh_ptr->_verts_world;	// use world(transformed) coordinates!
	float eps = 1e-4;   // less than 1e-4, then

						// counter-clockwise
	vec3f v0 = verts[vertexIndex[0]];
	vec3f v1 = verts[vertexIndex[1]];
	vec3f v2 = verts[vertexIndex[2]];

	// compute plane normal
	vec3f n = -cross(v1 - v0, v2 - v0);

	vec3f v0v1 = v1 - v0; vec3f v0v2 = v2 - v0;
	vec3f pvec = cross(r.d, v0v2);
	float det = dot(v0v1, pvec);
	// do not culling

	if (std::fabs(det < eps)) return false;

	float invDet = 1.0 / det;
	vec3f tvec = r.o - v0;
	float u = dot(tvec, pvec) * invDet;
	if (u < 0 || u > 1) return false;

	vec3f qvec = cross(tvec, v0v1);
	float v = dot(r.d, qvec) * invDet;
	if (v < 0 || u + v > 1) return false;

	rec.t = dot(v0v2, qvec) * invDet;
	rec.p = r.origin() + rec.t*r.direction();
	rec.obj = this;

	// TODO: special treatment for normals!
	if (!(mesh_ptr->_normals == NULL || mesh_ptr->nnorms == 0)) { // read-in normals
																  // normal intersection
																  // TODO
																  // use average instead now
																  //std::cout << "before: " << unit(n) << std::endl;
		n = (mesh_ptr->_normals_world[normalIndex[0]]
			+ mesh_ptr->_normals_world[normalIndex[1]]
			+ mesh_ptr->_normals_world[normalIndex[2]]) / 3.0;
		//std::cout << "after: " << n << std::endl << "===" << std::endl;
	}
	rec.norm = unit(n);
}

Material* Triangle::material() { return mesh_ptr->material; }

void Triangle::setVertexIndex(unsigned int i0, unsigned int i1, unsigned int i2) {
	vertexIndex[0] = i2; vertexIndex[1] = i1; vertexIndex[2] = i0;
}

void Triangle::setNormalIndex(unsigned int i0, unsigned int i1, unsigned int i2) {
	normalIndex[0] = i2; normalIndex[1] = i1; normalIndex[2] = i0;
}

void Triangle::setTxtIndex(unsigned int i0, unsigned int i1, unsigned int i2) {
	txtIndex[0] = i2; txtIndex[1] = i1; txtIndex[2] = i0;
}

void Triangle::barycentric(vec3f P, vec3f A, vec3f B, vec3f C, float &baryA, float &baryB, float &baryC) {
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

//////////////////////////////////////////////////////////////////////////
/// class Sphere
//////////////////////////////////////////////////////////////////////////
bool Sphere::intersect(const Ray& r, HitRecord& rec) {
	vec3f oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float disc = b*b - a*c;
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

//////////////////////////////////////////////////////////////////////////
/// class Cone
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/// class Plane
//////////////////////////////////////////////////////////////////////////