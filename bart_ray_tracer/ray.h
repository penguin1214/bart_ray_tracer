//
// Created by yang on 17-8-3.
//

#ifndef BART_RAY_TRACER_RAY_H
#define BART_RAY_TRACER_RAY_H

#include "vec3f.h"

class Ray {
public:
    vec3f o;
    vec3f d;

    Ray() {}
    Ray(const vec3f&a, const vec3f& b) :o(a), d(b){}
    vec3f origin() const { return o; }
    vec3f direction() const { return d; }
    vec3f point_at_parameter(float t) const { return o+d*t; }
};

#endif //BART_RAY_TRACER_RAY_H
