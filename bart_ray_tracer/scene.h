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

typedef float Vec2f[2];	// texture

vec3f reflect(const vec3f &normal, const vec3f &incident);
bool refract(const vec3f& v, const vec3f& norm, float ior, vec3f& refracted);

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

	vec3f trace(Ray& r, int depth, float incident_ior) {
		/*
		 * When light intersect an object, we compute radiance at that point.
		 * Objects are represented in geometry primitives like triangles, spheres, etc.
		 * Use Phong reflection model to shade non-transmissive surfaces. (local illumination)
		 * For translucent materials, transmission + reflection for dielectric materials, reflection for conductive materials.
		 */

		if (depth > max_depth) return vec3f(0.0);
		depth++;

		HitRecord record;
		if (! intersect(r, record)) {
			return vec3f(0.0);
		}

		vec3f col(0.0);

		// handle intersect color
		Material mat = record.obj->material();

		// light
		/*for (std::vector<Light* >::iterator it = lights.begin(); it != lights.end(); ++it) {

		}*/

		//////////////////////////////////////////////////////////////////////////
		/// FIRST COMPUTE LOCAL ILLUMINATION
		//////////////////////////////////////////////////////////////////////////

		// ambient
		//col += lights[0]->col * mat.ambient;

		// diffuse
		Light *light = lights[1]; // diffuse
		Ray r_scnd(record.p, unit(light->pos - record.p));
		HitRecord shadow_rec;
		if (!intersect(r_scnd, shadow_rec)) return lights[0]->col;	// if shadow, return ambient

		float tmp_cos = dot(r_scnd.direction(), record.norm);
		if (tmp_cos > 0) {
			if (mat.diffuse.x() > 0 || mat.diffuse.y() > 0 || mat.diffuse.z() > 0) {
				col += tmp_cos * mat.diffuse * light->col;
			}
		}

		// specular
		vec3f v_view = unit(camera->from - record.p);
		vec3f v_reflect = unit(reflect(record.norm, r.direction()));
		tmp_cos = dot(v_reflect, v_view);
		if (tmp_cos > 0.0) {
			col += mat.specular * std::pow(std::max(float(0.0), tmp_cos), mat.shine) * light->col;
		}

		//////////////////////////////////////////////////////////////////////////
		/// THEN COMPUTE GLOBAL ILLUMINATION
		/// RECURSIVELY CAST RAYS
		//////////////////////////////////////////////////////////////////////////

		/*
		// reflectance
		vec3f reflect_dir = reflect(record.norm, r.d);
		Ray r_reflect(record.p, reflect_dir);
		vec3f col_r = trace(r_reflect, depth, incident_ior);
		col += col_r * (1 - mat.T);

		// refraction
		if (mat.T > 0.0) {	// do transmit
			vec3f refract_dir;
			if (refract(r.d, record.norm, incident_ior / mat.ior, refract_dir)) {
				Ray r_refract(record.p, unit(refract_dir));
				vec3f col_t = trace(r_refract, depth, mat.ior);
				col += col_t * mat.T;
			}
			else {	// full reflect
				col += col_r * mat.T;
			}
		}
		*/

		/*
		// texture
		if (record.obj->mesh_ptr && record.obj->mesh_ptr->_txts) {
			// has texture
			Triangle *tri = dynamic_cast<Triangle *>(record.obj);
			vec3f A = tri->mesh_ptr->verts[tri->vertexIndex[0]];
			vec3f B = tri->mesh_ptr->verts[tri->vertexIndex[1]]; 
			vec3f C = tri->mesh_ptr->verts[tri->vertexIndex[2]];
			vec3f P = record.p;

			float *tA = tri->mesh_ptr->_txts[tri->txtIndex[0]];
			float *tB = tri->mesh_ptr->_txts[tri->txtIndex[1]];
			float *tC = tri->mesh_ptr->_txts[tri->txtIndex[2]];

			float baryA, baryB, baryC;
			// TODO barycentric coordinates are not corresponding to original ones??
			Triangle::barycentric(P, A, B, C, baryB, baryC, baryA);

			float tP[2];
			tP[0] = baryA*tA[0] + baryB*tB[0] + baryC*tC[0];
			tP[1] = baryA*tA[1] + baryB*tB[1] + baryC*tC[1];

			// scale
			tP[0] = Texture::scale(tP[0]);
			tP[1] = Texture::scale(tP[1]);

			int tmp_u = tP[0] * tri->mesh_ptr->texture->mWidth;
			int tmp_v = tP[1] * tri->mesh_ptr->texture->mHeight;

			int tmp_idx = tmp_v * tri->mesh_ptr->texture->mWidth + tmp_u;
			tmp_idx *= 3;

			col.e[0] += (float)tri->mesh_ptr->texture->mRGB[tmp_idx + 0] / 255.0f;
			col.e[1] += (float)tri->mesh_ptr->texture->mRGB[tmp_idx + 1] / 255.0f;
			col.e[2] += (float)tri->mesh_ptr->texture->mRGB[tmp_idx + 2] / 255.0f;
		}
		*/

		return col;
	}

	bool intersect(Ray& r, HitRecord& rec) {
		rec.t = 100;
		for (std::vector<Shape* >::iterator it = shapes.begin(); it != shapes.end(); ++it) {
			HitRecord tmpRec;
			if ((*it)->intersect(r, tmpRec) && tmpRec.t < rec.t) {
				rec = tmpRec;
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
