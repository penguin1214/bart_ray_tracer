//
// Created by yang on 17-8-3.
//

#ifndef BART_RAY_TRACER_SCENE_H
#define BART_RAY_TRACER_SCENE_H

#include <vector>
#include <algorithm>
#include <cmath>
#include "shape.h"
#include "light.h"
#include "camera.h"
#include "vec3f.h"
#include "animation.h"

vec3f reflect(const vec3f &normal, const vec3f &incident);

class Scene {
public:
	Camera *camera;
	std::vector<Shape*> shapes;
	std::vector<Light*> lights;
	vec3f background;
	AnimationList* gAnimations;
	int max_depth;

	Scene() {
		gAnimations = NULL;
		camera = new Camera();
		background = vec3f(0.0);
		max_depth = 0;
	}

	vec3f trace(Ray& r, int depth) {
		HitRecord record;
		if (! intersect(r, record)) {
			return vec3f(0.05);
		}

		vec3f col(0.0);

		// handle intersect color
		Material mat = record.obj->material();
		col += record.obj->material().ambient;

		// light
		/*for (std::vector<Light* >::iterator it = lights.begin(); it != lights.end(); ++it) {

		}*/

		// ambient
		col += lights[0]->col * mat.ambient;

		// diffuse
		Light *light = lights[1]; // diffuse
		Ray r_scnd(record.p, unit(light->pos - record.p));
		float tmp_cos = dot(r_scnd.direction(), record.norm);
		if (tmp_cos > 0) {
			if (mat.diffuse.x() > 0 || mat.diffuse.y() > 0 || mat.diffuse.z() > 0) {
				col += tmp_cos * mat.diffuse * light->col;
			}
		}

		// specular
		vec3f v_view = unit(camera->at - record.p);
		vec3f v_reflect = unit(reflect(record.norm, r.direction()));
		tmp_cos = dot(v_reflect, v_view);
		col += mat.specular * std::pow(std::max(float(0.0), dot(v_view, v_reflect)), 1000) * light->col;

		return col;
	}

	bool intersect(Ray& r, HitRecord& rec) {
		rec.t = 100;
		for (std::vector<Shape* >::iterator it = shapes.begin(); it != shapes.end(); ++it) {
			HitRecord tmpRec;
			/*if ((*it)->intersect(r, tmpRec) && tmpRec.t < rec.t) {
				rec = tmpRec;
			}*/
			if ((*it)->intersect(r, tmpRec)) {
				if (tmpRec.t < rec.t) {
					rec = tmpRec;
				}
			}
		}

		if (rec.t < 100) {
			return true;
		}
		else {
			return false;
		}
	}

	void setBackground(vec3f c) {
		background = vec3f(c.r(), c.g(), c.b());
	}

	void addShape(Shape* shape) { shapes.push_back(shape); }

	void addLight(Light* l) { lights.push_back(l); }
};
#endif //BART_RAY_TRACER_SCENE_H
