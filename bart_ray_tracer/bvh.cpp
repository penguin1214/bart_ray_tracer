#include "bvh.h"

#include <math.h>

#include "shape.h"
#include "mesh.h"
#include "ray.h"

//////////////////////////////////////////////////////////////////////////
/// class Extents
//////////////////////////////////////////////////////////////////////////
Extents::Extents() {
	for (int i = 0; i < G_PLANE_SET_NORMAL_NUM; i++) {
		d[i][0] = INFINITY; d[i][1] = -INFINITY;
	}
}

bool Extents::intersect(Ray &r, ExtentHitRecord &rec, float *ndoto, float *ndotr) {
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

void Extents::extendBy(Extents &ext) {
	for (int i = 0; i < G_PLANE_SET_NORMAL_NUM; i++) {
		if (d[i][0] > ext.d[i][0]) d[i][0] = ext.d[i][0];
		if (d[i][1] < ext.d[i][1]) d[i][1] = ext.d[i][1];
	}
}

vec3f Extents::centroid() {
	return vec3f(
		d[0][0] + d[0][1] * 0.5,
		d[1][0] + d[1][1] * 0.5,
		d[2][0] + d[2][1] * 0.5
	);
}

//////////////////////////////////////////////////////////////////////////
/// class OctreeNode
//////////////////////////////////////////////////////////////////////////
OctreeNode::OctreeNode() { _bbox = new BBox(); }

OctreeNode::~OctreeNode() {}
//////////////////////////////////////////////////////////////////////////
/// class Octree
//////////////////////////////////////////////////////////////////////////
Octree::Octree() { _root = nullptr; }
Octree::~Octree() { deleteOctree(_root); }
void Octree::init() { _root = new OctreeNode(); }
void Octree::build() { build(_root); }
void Octree::insert(Extents *ext) { insert(_root, ext, 0); }

/* Insert node to Octree. */
void Octree::insert(OctreeNode *node, Extents *ext, int node_depth) {
	// if root is leaf
	// 		insert in root node
	// else iterate 8 children
	//		if ext within child ext
	//			child.insert(ext)
	// recursive...

	if (node_depth == _max_depth) {
		// just add extents
		node->extentsList.push_back(ext);
	}
	else {
		// not max depth
		if (node->extentsList.size() == 0) {
			// not occupied yet, insert
			node->extentsList.push_back(ext);
			// compute bbox for child node

		}
		else {
			// already occupied
			// split
			for (int i = 0; i < 8; i++) {
				node->children[i] = new OctreeNode();

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
			}

		}
	}
#if 0
	if (node->_is_leaf) {
		if (node->extentsList.size() == 0 || node_depth == _max_depth) {
			node->extentsList.push_back(ext);
		}
		else {
			// has children
			// set inserted node's box and split node
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
			if (node->children[insert_idx] == nullptr) node->children[insert_idx] = new OctreeNode();
			node->children[insert_idx]->_bbox = &child_bbox;
			std::cout << node_depth << std::endl;
			insert(node->children[insert_idx], ext, node_depth + 1);
		}
		insert(node, ext, node_depth);
	}
	else {
		// reach depth
		if (node->extentsList.size() == 0) {
			// add done
			node->_is_leaf = true;	// fix
		}
		else {
			node->_is_leaf = false;
			insert(node, node->extentsList.back(), node_depth);
			node->extentsList.pop_back();
		}
	}

#endif
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

//////////////////////////////////////////////////////////////////////////
/// class BVH
//////////////////////////////////////////////////////////////////////////
const vec3f BVH::_plane_set_normals[BVH::n_plane_set_normal] = {
	vec3f(1, 0, 0),
	vec3f(0, 1, 0),
	vec3f(0, 0, 1),
	vec3f(sqrtf(3) / 3.f,  sqrtf(3) / 3.f, sqrtf(3) / 3.f),
	vec3f(-sqrtf(3) / 3.f,  sqrtf(3) / 3.f, sqrtf(3) / 3.f),
	vec3f(-sqrtf(3) / 3.f, -sqrtf(3) / 3.f, sqrtf(3) / 3.f),
	vec3f(sqrtf(3) / 3.f, -sqrtf(3) / 3.f, sqrtf(3) / 3.f)
};

BVH::BVH(std::vector<Mesh* > meshes) {
	std::cout << "Initializing BVH tree..." << std::endl;
	_extents.reserve(meshes.size());
	for (int i = 0; i < meshes.size(); i++) {
		_extents.push_back(new Extents);
	}

	for (int i = 0; i < meshes.size(); i++) {
		for (int j = 0; j < n_plane_set_normal; j++) {
			// compute extents for every mesh
			meshes[i]->computeExtent(n_plane_set_normal, _plane_set_normals, _extents[i]);
		}
		_extents[i]->_mesh_ptr = meshes[i];
	}

	// TODO: compute bbox for root node, do not use extent
	_octree = new Octree();
	_octree->init();

	for (int i = 0; i < meshes.size(); i++) {
		// recursively insert node
		_octree->insert(_extents[i]);
	}

	std::cout << "Initialization done. Updating BVH..." << std::endl;
	// build from bottom up
	_octree->build();
}
BVH::~BVH() {}

bool BVH::intersect(Ray &r, HitRecord &rec) {
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

bool BVH::intersect(OctreeNode *node, Ray &r, HitRecord &rec, float *ndoto, float *ndotr) {
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