//
// Created by yang on 17-8-4.
//

#ifndef BART_RAY_TRACER_MESH_H
#define BART_RAY_TRACER_MESH_H

#include <vector>
#include "assert.h"
#include "vec3f.h"
#include "material.h"
#include "transform.h"
#include "matrix4x4.h"

class Triangle;

class Mesh {
public:
	uint32_t nverts;
	bool _is_static;
	vec3f *verts; // do not use pointer here because of the copy issue
	vec3f *_verts_world;
	Material material;
	TransformHierarchy *_trans_hierarchy;
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

	void updateVertex() {
		assert(verts != NULL && nverts >= 3);
		for (int i = 0; i < nverts; i++) {
			vec3f v(verts[i]);
			v = _trans_local_to_world * v;
			_verts_world[i] = v;
		}
	}

	void addTriangle(Triangle* tri) { triangles.push_back(tri); }

	void setTransformHierarchy(TransformHierarchy* trans) {
		_trans_hierarchy = trans;
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
