//
// Created by yang on 17-8-4.
//

#ifndef BART_RAY_TRACER_MESH_H
#define BART_RAY_TRACER_MESH_H

#include <vector>
#include "assert.h"

#include "core.h"
#include "transform.h"
#include "matrix4x4.h"

class Material;
class Texture;
class Triangle;
class BVH;
class Extents;
class Ray;
struct HitRecord;

class Mesh {
public:
	uint32_t nverts, nnorms, ntxts;
	bool _is_static;
	vec3f *verts;
	vec3f *_verts_world;
	vec3f *_normals;
	vec3f *_normals_world;
	Vec2f *_txts;
	Material *material;
	Texture *texture;
	//TransformHierarchy *_trans_hierarchy;
	Matrix4x4 _trans_local_to_world;
	Matrix4x4 _trans_local_to_world_inv;

	std::vector<Triangle* > triangles;

	// constructors
	Mesh(uint32_t nv, vec3f *v);
	Mesh(uint32_t nv, vec3f *v, uint32_t nn, vec3f *n, uint32_t nt, Vec2f *t);

	void updateVertex();
	void addTriangle(Triangle* tri);
	void setMeshTransform(TransformHierarchy* trans);

	//////////////////////////////////////////////////////////////////////////
	/// Methods related with BVH
	//////////////////////////////////////////////////////////////////////////

	// every mesh holds a pointer to its extent
	void computeExtent(int n_pls_norm, const vec3f *plane_set_norms, Extents *ext);
	inline void computeExtD(const vec3f &plane_norm, float d[2]);
	bool intersect(Ray &r, HitRecord &rec);
};

#endif //BART_RAY_TRACER_MESH_H
