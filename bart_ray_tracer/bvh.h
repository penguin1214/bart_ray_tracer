#ifndef BART_RAY_TRACER_BVH_H
#define BART_RAY_TRACER_BVH_H

#include <memory>

#include "core.h"
#include "ray.h"

class Mesh;
struct HitRecord;
struct ExtentHitRecord;

class Extents {
public:
	Extents() {
		for (int i = 0; i < G_PLANE_SET_NORMAL_NUM; i++) {
			d[i][0] = -INFINITY; d[i][1] = INFINITY;
		}
	}

	inline bool intersect(Ray &r, ExtentHitRecord &rec, float *ndoto, float *ndotr) {
		// test against all planes
		float tnear, tfar;
		for (int i = 0; i < G_PLANE_SET_NORMAL_NUM; i++) {
			tnear = (d[i][0] - ndoto[i]) / ndotr[i];
			tfar = (d[i][1] - ndoto[i]) / ndotr[i];
			if (ndotr[i] < 0) std::swap(tnear, tfar);
			// update
			if (tnear > rec.t_near) {
				rec.t_near = tnear; rec.plane_idx = i;
			}
			if (tfar < rec.t_far) rec.t_far = tfar;
			if (tfar < tnear) return false;
		}
		return true;
	}

	void extendBy(Extents &ext) {
		for (int i = 0; i < G_PLANE_SET_NORMAL_NUM; i++) {
			if (d[i][0] > ext.d[i][0]) d[i][0] = ext.d[i][0];
			if (d[i][1] < ext.d[i][1]) d[i][1] = ext.d[i][1];
		}
	}

	inline vec3f centroid() {
		return vec3f(
			d[0][0] + d[0][1] * 0.5,
			d[1][0] + d[1][1] * 0.5,
			d[2][0] + d[2][1] * 0.5
		);
	}

	float d[G_PLANE_SET_NORMAL_NUM][2];
	std::shared_ptr<Mesh> _mesh_ptr;
};


struct OctreeNode {
	OctreeNode *children[8] = { nullptr };
	std::vector<Extents *> extentsList;	// every node holds a set of extents inside it
	Extents nodeExtent;
	BBox *_bbox;	// bbox is only used in tree construction, cost memory, consider optimization.
	bool _is_leaf = true;
};

/* Octree instances hold a pointer to the root node,
* and hold Octree methods.
*/
class Octree {
public:
	Octree() { _root = nullptr; }
	~Octree() { deleteOctree(_root); }

	void build() { build(_root); }
	void insert(Extents *ext) { insert(_root, ext, 0); }

	const int _max_depth = G_BVH_DEPTH;
	OctreeNode *_root;

private:
	void insert(OctreeNode *node, Extents *ext, int node_depth);
	void build(OctreeNode *node);
	void deleteOctree(OctreeNode *node);
};

/* Insert node to Octree. */
void Octree::insert(OctreeNode *node, Extents *ext, int node_depth) {
	// if root is leaf
	// 		insert in root node
	// else iterate 8 children
	//		if ext within child ext
	//			child.insert(ext)
	// recursive...
	if (node->_is_leaf) {
		if (node->extentsList.size() == 0 || node_depth == _max_depth) {
			node->extentsList.push_back(ext);
		}
		else {
			// split node and RE-INSERT!
			node->_is_leaf = false;
			insert(node, node->extentsList.back(), node_depth);
			node->extentsList.pop_back();
		}
		insert(node, ext, node_depth);
	}
	else {
		// has children
		BBox child_bbox;
		vec3f ext_centroid = ext->centroid();
		vec3f node_centroid = node->nodeExtent.centroid();
		int insert_idx = 0;
		// x-axis
		if (ext_centroid.x() > node_centroid.x()) {
			// right side
			insert_idx = 4;
			child_bbox.p_min.e[0] = (node->_bbox->p_min.e[0] + node->_bbox->p_max.e[0]) / 2.0;
			child_bbox.p_max.e[0] = node->_bbox->p_max.e[0];
		}
		else {
			// left side
			child_bbox.p_min.e[0] = node->_bbox->p_min.e[0];
			child_bbox.p_max.e[0] = (node->_bbox->p_min.e[0] + node->_bbox->p_max.e[0]) / 2.0;
		}
		// y-axis
		if (ext_centroid.y() > node_centroid.y()) {
			// up side
			insert_idx += 2;
			child_bbox.p_min.e[1] = (node->_bbox->p_min.e[1] + node->_bbox->p_max.e[1]) / 2.0;
			child_bbox.p_max.e[1] = node->_bbox->p_max.e[1];
		}
		else {
			// down side
			child_bbox.p_min.e[1] = node->_bbox->p_min.e[1];
			child_bbox.p_max.e[1] = (node->_bbox->p_min.e[1] + node->_bbox->p_max.e[1]) / 2.0;
		}
		// z-axis
		if (ext_centroid.z() > node_centroid.z()) {
			// far side
			insert_idx += 1;
			child_bbox.p_min.e[2] = (node->_bbox->p_min.e[2] + node->_bbox->p_max.e[2]) / 2.0;
			child_bbox.p_max.e[2] = node->_bbox->p_max.e[2];
		}
		else {
			// near side
			child_bbox.p_min.e[2] = node->_bbox->p_min.e[2];
			child_bbox.p_max.e[2] = (node->_bbox->p_min.e[2] + node->_bbox->p_max.e[2]) / 2.0;
		}

		// do insert
		if (node->children[insert_idx] == nullptr) node->children[insert_idx] = new OctreeNode;
		node->children[insert_idx]->_bbox = &child_bbox;
		insert(node->children[insert_idx], ext, node_depth + 1);
	}
}

void Octree::deleteOctree(OctreeNode *node) {
	for (int i = 0; i < 8; i++) {
		if (node->children[i] != nullptr) deleteOctree(node->children[i]);
	}
	delete node;
}

/* All extents have been inserted to the tree.
* build() builds the tree from bottom to up, compute extents for all nodes along the way.
*/
void Octree::build(OctreeNode *node) {
	if (_root->_is_leaf) {
		for (const auto &e : node->extentsList) {	// access by reference, e is a reference of pointer
			node->nodeExtent.extendBy(*e);	// update extent by testing all extents in the node
		}
	}
	else {
		// iterate through all children
		for (int i = 0; i < 8; i++) {
			if (node->children[i] != nullptr) {
				build(node->children[i]);
				node->nodeExtent.extendBy(node->children[i]->nodeExtent);
			}
		}
	}
}


/* A scene have one BVH instance.
* And a BVH instance holds a pointer to a Octree.
*/
class BVH {
public:
	BVH() : _octree(nullptr) {}
	BVH(std::vector< std::shared_ptr<Mesh> > meshes) {
		_extents.reserve(meshes.size());

		for (int i = 0; i < meshes.size(); i++) {
			for (int j = 0; j < n_plane_set_normal; j++) {
				// compute extents for every mesh
				meshes[i]->computeExtent(n_plane_set_normal, _plane_set_normals, _extents[i]);
			}
			_extents[i]->_mesh_ptr = meshes[i];
		}

		// TODO: compute bbox for root node, do not use extent
		_octree = new Octree();

		for (int i = 0; i < meshes.size(); i++) {
			// recursively insert node
			_octree->insert(_extents[i]);
		}

		// build from bottom up
		_octree->build();
	}
	~BVH() {}

	// directly return a low-level intersect primitive
	bool intersect(Ray &r, HitRecord &rec) {
		// test from top to bottom
		float ndoto[G_PLANE_SET_NORMAL_NUM], ndotr[G_PLANE_SET_NORMAL_NUM];
		for (int i = 0; i < G_PLANE_SET_NORMAL_NUM; i++) {
			ndoto[i] = dot(_plane_set_normals[i], r.o);
			ndotr[i] = dot(_plane_set_normals[i], r.d);
		}

		HitRecord tmpRec;
		if (intersect(_octree->_root, r, tmpRec, ndoto, ndotr) && tmpRec.t < rec.t) {
			rec = tmpRec; return true;
		}
		else {
			return false;
		}
	}

	std::vector<Extents*> _extents;
	Octree *_octree;

	static const int n_plane_set_normal = G_PLANE_SET_NORMAL_NUM;
	static const vec3f _plane_set_normals[n_plane_set_normal];

private:
	bool intersect(OctreeNode *node, Ray &r, HitRecord &rec, float *ndoto, float *ndotr) {
		bool flag = false;
		// intersect method for every node
		ExtentHitRecord tmpExtRec;
		if (!node->nodeExtent.intersect(r, tmpExtRec, ndoto, ndotr)) return false;

		HitRecord tmpRec;
		if (node->_is_leaf) {
			// return the nearest intersection info in this node
			for (const auto &e : node->extentsList) {
				// return the nearest triangle intersection info
				if (e->_mesh_ptr->intersect(r, tmpRec) && tmpRec.t < rec.t) {
					rec = tmpRec;
					flag = true;
				}
			}
		}
		else {
			// test children
			for (int i = 0; i < 8; i++) {
				if (node->children[i] != nullptr) {
					if (intersect(node->children[i], r, tmpRec, ndoto, ndotr) && tmpRec.t < rec.t) {
						rec = tmpRec; flag = true;
					}
				}
			}
		}
	}
};

const vec3f BVH::_plane_set_normals[BVH::n_plane_set_normal] = {
	vec3f(1, 0, 0),
	vec3f(0, 1, 0),
	vec3f(0, 0, 1),
	vec3f(sqrtf(3) / 3.f,  sqrtf(3) / 3.f, sqrtf(3) / 3.f),
	vec3f(-sqrtf(3) / 3.f,  sqrtf(3) / 3.f, sqrtf(3) / 3.f),
	vec3f(-sqrtf(3) / 3.f, -sqrtf(3) / 3.f, sqrtf(3) / 3.f),
	vec3f(sqrtf(3) / 3.f, -sqrtf(3) / 3.f, sqrtf(3) / 3.f)
};


#endif
