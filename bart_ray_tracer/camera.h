//
// Created by yang on 17-8-3.
//

#ifndef BART_RAY_TRACER_CAMERA_H
#define BART_RAY_TRACER_CAMERA_H

#include "vec3f.h"
#include "ray.h"
#include "film.h"
#include "utility.h"

class Camera {
public:
	vec3f from;
	vec3f at;
	vec3f forward;
	vec3f up;
	vec3f right;
	float vfov; // vertical field of view
	float near_plane_distance;
	vec3f up_left_corner;
	vec3f horizontal;
	vec3f vertical;
	Film* film;
	//vec3f u, v, w;

	Camera() {
		film = new Film();
	}
	Camera(int wi, int h, vec3f f, vec3f a, vec3f u, float vf): from(f), at(a), up(u), vfov(vf) {
		film->width = wi; film->height = h;
		forward = unit(at - from);
		right = unit(cross(up, forward));
		up = unit(cross(forward, right));
		at = unit(at);

		float theta = deg2rad(vfov);
		near_plane_distance = h / (2 * tan(theta / 2));

		up_left_corner = from + near_plane_distance*forward - film->width / 2 * right + film->height / 2 * up;
		horizontal = film->width*right;
		vertical = film->height*up;
	}

	void setCamParam(vec3f f, vec3f a, vec3f u, float vf) {
		from = f; at = a; up = u; vfov = vf;
		forward = unit(at - from);
		right = unit(cross(up, forward));
		up = unit(cross(forward, right));
		at = unit(at);

		float theta = deg2rad(vfov);
		near_plane_distance = film->height / (2 * tan(theta / 2));
		up_left_corner = from + near_plane_distance*forward - film->width / 2 * right + film->height / 2 * up;
		horizontal = film->width*right;
		vertical = film->height*up;
	}

	Ray get_ray(float u, float v) {
		//std::cout << "from: " << from << std::endl;
		//std::cout << "cursor: " << lower_left_corner + u*horizontal + v*vertical;
		return Ray(from, unit(up_left_corner+u*horizontal-v*vertical-from));
	}
};

#endif //BART_RAY_TRACER_CAMERA_H
