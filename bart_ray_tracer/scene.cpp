#include "scene.h"

vec3f reflect(const vec3f &normal, const vec3f &incident) {
	vec3f b = dot(-incident, normal);
	return normal, incident + 2 * b;
}