#ifndef BART_RAY_TRACER_BVH_H
#define BART_RAY_TRACER_BVH_H

#include <vector>
#include <memory>

#include "core.h"

class Mesh;
class Ray;
class BBox;
struct HitRecord;
struct ExtentHitRecord;

class Extents {
public:
	Extents();

	inline bool intersect(Ray &r, ExtentHitRecord &rec, float *ndoto, float *ndotr);
	void extendBy(Extents &ext);
	inline vec3f centroid();

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
	Octree();
	~Octree();

	void build();
	void insert(Extents *ext);

	const int _max_depth = G_BVH_DEPTH;
	OctreeNode *_root;

private:
	void insert(OctreeNode *node, Extents *ext, int node_depth);
	void build(OctreeNode *node);
	void deleteOctree(OctreeNode *node);
};


/* A scene have one BVH instance.
* And a BVH instance holds a pointer to a Octree.
*/
class BVH {
public:
	BVH() : _octree(nullptr) {}
	BVH(std::vector< std::shared_ptr<Mesh> > meshes);
	~BVH();

	// directly return a low-level intersect primitive
	bool intersect(Ray &r, HitRecord &rec);

	std::vector<Extents*> _extents;
	Octree *_octree;

	static const int n_plane_set_normal = G_PLANE_SET_NORMAL_NUM;
	static const vec3f _plane_set_normals[n_plane_set_normal];

private:
	bool intersect(OctreeNode *node, Ray &r, HitRecord &rec, float *ndoto, float *ndotr);
};

#endif
