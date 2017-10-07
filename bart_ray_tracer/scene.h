//
// Created by yang on 17-8-3.
//

#ifndef BART_RAY_TRACER_SCENE_H
#define BART_RAY_TRACER_SCENE_H

#include <vector>
#include <algorithm>
#include <cmath>

#include "core.h"
#include "bvh.h"
#include "light.h"
#include "camera.h"
#include "animation.h"

class Material;
class Mesh;
class Shape;
struct HitRecord;

typedef float Vec2f[2];	// texture

vec3f reflect(const vec3f &normal, const vec3f &incident);
bool refract(const vec3f& v, vec3f& norm, float ior, vec3f& refracted);

class Scene {
public:
	Camera *camera;
	std::vector<Mesh*> meshes;
	std::vector<Shape*> shapes;
	std::vector<Light*> lights;
	vec3f background;
	AnimationList* gAnimations;
	BVH *bvh;

	Scene();

	vec3f trace(Ray& r, int depth, float incident_ior);
	bool intersect(Ray& r, HitRecord& rec);
	void setBackground(vec3f c);
	void addMesh(Mesh* m);
	void addShape(Shape* shape);
	void addLight(Light* l);
};
#endif //BART_RAY_TRACER_SCENE_H
