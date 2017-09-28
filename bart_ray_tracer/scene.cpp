#include "scene.h"

vec3f reflect(const vec3f &normal, const vec3f &incident) {
	vec3f b = dot(-incident, normal);
	return unit(incident + 2 * b);
}

bool refract(const vec3f& v, const vec3f& norm, float ior, vec3f& refracted) {
	vec3f unit_v = unit(v);
	float dt = dot(unit_v, norm);
	float discriminant = 1.0 - ior*ior*(1 - dt*dt);
	if (discriminant > 0) {
		refracted = ior*(v - norm*dt) - norm*sqrt(discriminant);
		return true;
	}
	else {
		return false;
	}
}