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
    vec3f up;
    float vfov; // vertical field of view
    vec3f lower_left_corner;
    vec3f horizontal;
    vec3f vertical;
    Film* film;
    vec3f u, v, w;

    Camera() {
        film = new Film();
    }
    Camera(int wi, int h, vec3f f, vec3f a, vec3f u, float vf): from(f), at(a), up(u), vfov(vf) {
        film->width = wi; film->height = h;
        float theta = deg2rad(vfov);
        float aspect = film->width/ film->height;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;
        w = unit(from-at);
        u = unit(cross(up, w));
        v = unit(cross(w, u));
        lower_left_corner = vec3f(-half_width, -half_height, -1.0);
        lower_left_corner = from - half_width*u = half_height*v - w;
        horizontal = 2*half_width*u;
        vertical = 2*half_height*v;
    }

    void setCamParam(vec3f f, vec3f a, vec3f u, float vf) {
        from = f; at = a; up = u; vfov = vf;
        float theta = deg2rad(vfov);
        float aspect = film->width/ film->height;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;
        w = unit(from-at);
        u = unit(cross(up, w));
        v = unit(cross(w, u));
        lower_left_corner = vec3f(-half_width, -half_height, -1.0);
        lower_left_corner = from - half_width*u = half_height*v - w;
        horizontal = 2*half_width*u;
        vertical = 2*half_height*v;
    }

    Ray get_ray(float u, float v) {
        return Ray(from, lower_left_corner+u*horizontal+v*vertical-from);
    }
};

#endif //BART_RAY_TRACER_CAMERA_H
