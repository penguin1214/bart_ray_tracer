#include "mesh.h"
#include "bvh.h"
#include "shape.h"

//////////////////////////////////////////////////////////////////////////
/// class Mesh
//////////////////////////////////////////////////////////////////////////
Mesh::Mesh(uint32_t nv, vec3f *v) {
	_is_static = true;
	nverts = nv; _verts_world = v;
	// do deep copy
	verts = (vec3f*)malloc(nverts * sizeof(vec3f));
	memcpy(verts, _verts_world, nverts * sizeof(vec3f));
}

Mesh::Mesh(uint32_t nv, vec3f *v, uint32_t nn, vec3f *n, uint32_t nt, Vec2f *t) {
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

void Mesh::updateVertex() {
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

void Mesh::addTriangle(Triangle* tri) { triangles.push_back(tri); }

void Mesh::setMeshTransform(TransformHierarchy* trans) {
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

void Mesh::computeExtent(int n_pls_norm, const vec3f *plane_set_norms, Extents *ext) {
	for (int i = 0; i < n_pls_norm; i++) {
		computeExtD(plane_set_norms[i], ext[i].d[i]);
	}
}

void Mesh::computeExtD(const vec3f &plane_norm, float d[2]) {
	float td;
	for (int i = 0; i < nverts; i++) {
		td = dot(plane_norm, _verts_world[i]);
		if (td < d[0]) d[0] = td;
		if (td > d[1]) d[1] = td;
	}
}

bool Mesh::intersect(Ray &r, HitRecord &rec) {
	bool flag = false;
	for (const auto &e : triangles) {
		HitRecord tmpRec;
		if (e->intersect(r, rec) && tmpRec.t < rec.t) {
			rec = tmpRec;
			flag = true;
		}
	}
	return flag;
}