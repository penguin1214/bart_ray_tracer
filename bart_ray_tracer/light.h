//
// Created by yang on 17-8-3.
//

#ifndef BART_RAY_TRACER_LIGHT_H
#define BART_RAY_TRACER_LIGHT_H

class Light {
public:
	vec3f pos;
	vec3f col;
};

class PointLight : public Light {
public:
	PointLight(vec3f p, vec3f c) {
		pos = p; col = c;
	}
};

class AmbientLight : public Light {
public:
	AmbientLight(vec3f c) {
		col = c;
	}
};

#endif //BART_RAY_TRACER_LIGHT_H
