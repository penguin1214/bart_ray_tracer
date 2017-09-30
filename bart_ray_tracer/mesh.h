//
// Created by yang on 17-8-4.
//

#ifndef BART_RAY_TRACER_MESH_H
#define BART_RAY_TRACER_MESH_H

#include <vector>
#include "assert.h"
#include "common.h"
#include "vec3f.h"
#include "material.h"
#include "texture.h"
#include "transform.h"
#include "matrix4x4.h"

class Triangle;

class Mesh {
public:
	uint32_t nverts, nnorms, ntxts;
	bool _is_static;
	vec3f *verts;
	vec3f *_verts_world;
	vec3f *_normals;
	vec3f *_normals_world;
	Vec2f *_txts;
	Material material;
	Texture *texture;
	//TransformHierarchy *_trans_hierarchy;
	Matrix4x4 _trans_local_to_world;
	Matrix4x4 _trans_local_to_world_inv;
	
	std::vector<Triangle* > triangles;

	Mesh(uint32_t nv, vec3f *v) {
		_is_static = true;
		nverts = nv; _verts_world = v;
		// do deep copy
		verts = (vec3f*)malloc(nverts * sizeof(vec3f));
		memcpy(verts, _verts_world, nverts * sizeof(vec3f));
	}

	Mesh(uint32_t nv, vec3f *v, uint32_t nn, vec3f *n, uint32_t nt, Vec2f *t) {
		_is_static = true;
		nverts = nv; nnorms = nn; ntxts = nt;
		_verts_world = v; _normals_world = n; _txts = t;
		verts = (vec3f*)malloc(nverts * sizeof(vec3f));
		memcpy(verts, _verts_world, nverts * sizeof(vec3f));

		if (_normals_world) {
			_normals = (vec3f*)malloc(nnorms * sizeof(vec3f));
			memcpy(_normals, _normals_world, nn * sizeof(vec3f));
		}

		/* TODO: deep copy */
		/*_txts = (Vec2f*)malloc(ntxts * sizeof(Vec2f));
		memcpy(_txts, t, nt * sizeof(Vec2f));*/
	}

	void updateVertex() {
		assert(verts != NULL && nverts >= 3);
		for (int i = 0; i < nverts; i++) {
			vec3f v(verts[i]);
			//std::cout << "local coord: " << v << std::endl;
			v = _trans_local_to_world * v;
			_verts_world[i] = v;
			//std::cout << "world coord: " << v << std::endl;
			//std::cout << "=================" << std::endl;
		}

		if (_normals) {
			assert(nnorms > 0);
			for (int i = 0; i < nnorms; i++) {
				vec3f n(_normals[i]);
				// NOTE NORMAL TRANSFORMATION!!!
				n = _trans_local_to_world_inv.transpose() * n;
				n = unit(n);
				_normals_world[i].e[0] = n.x(); _normals_world[i].e[1] = n.y(); _normals_world[i].e[2] = n.z();
			}
		}
	}

	void addTriangle(Triangle* tri) { triangles.push_back(tri); }

	void setMeshTransform(TransformHierarchy* trans) {
		//_trans_hierarchy = trans;
		if (trans == NULL) return;

		for (TransformHierarchy *t_tmp = trans; t_tmp != NULL; t_tmp = t_tmp->_parent) {
			if (!t_tmp->_is_static) {
				// mesh is not static
				_is_static = false;
				return; // must return!
			}
			/* TODO: world transform? */
			_trans_local_to_world = t_tmp->_transformMatrix * _trans_local_to_world;
		}
		_trans_local_to_world_inv = _trans_local_to_world.inverse();
		updateVertex();
	}
};

#endif //BART_RAY_TRACER_MESH_H
