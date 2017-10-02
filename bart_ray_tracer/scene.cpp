#include "scene.h"

vec3f reflect(const vec3f &normal, const vec3f &incident) {
	vec3f b = dot(-incident, normal);
	return unit(incident + 2 * b);
}

bool refract(const vec3f& v, vec3f& norm, float eta, vec3f& refracted) {
	// eta = sin(theta2) / sin(theta1) = incident_ior / material_ior
	// v is normalized ray direction
	// norm is normalized normal vector
	// refracted = (v + cos(theta1) * norm)*ior - norm*cos(theta2)
	// cos(theta1) = v * norm
	// cos(theta2) = sqrt(1 - sin(theta2)^2)
	// sin(theta2) = ior * sin(theta1) = ior * sqrt(1 - cos(theta1)^2)
	vec3f N = norm;
	float cos1 = dot(v, N);
	if (cos1 < 0.0) {
		cos1 = -cos1;
	}
	else {
		eta = 1.0 / eta; N = -N;
	}
	float k = 1.0 - eta*eta*(1.0 - cos1*cos1);
	if (k < 0.0) {
		return false;	// total reflection
	}
	else {
		refracted = (v + cos1 * N) * eta - N * sqrtf(k);
		return true;
	}
}

//bool refract(const vec3f& v, const vec3f& norm, float ior, vec3f& refracted) {
//	vec3f unit_v = unit(v);
//	float dt = dot(unit_v, norm);
//	float discriminant = 1.0 - ior*ior*(1 - dt*dt);
//	if (discriminant > 0) {
//		refracted = ior*(v - norm*dt) - norm*sqrt(discriminant);
//		return true;
//	}
//	else {
//		return false;
//	}
//}