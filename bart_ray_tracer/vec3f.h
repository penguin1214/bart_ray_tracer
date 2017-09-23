//
// Created by yang on 17-8-3.
//

#ifndef BART_RAY_TRACER_VEC3F_H
#define BART_RAY_TRACER_VEC3F_H

/*
 * @member: x, y, z => e[3] | x(), y(), z()
 * @methods: uniform(), length()
 * @operator reloading: vec3(+)vec3, vec3(-)vec3, scalar(*)vec3, vec3(*)scalar,
 *                      vec3(*)vec3, -(vec3)
 */
#include <iostream>
#include <cmath>

class vec3f {

public:
	// constructor
	vec3f() {}

	vec3f(float e0, float e1, float e2) {
		e[0] = e0; e[1] = e1; e[2] = e2;
	}
	vec3f(float f) {
		e[0] = f;
		e[1] = f; e[2] = f;
	}
	vec3f(float f[3]) { e[0] = f[0], e[1] = f[1], e[2] = f[2]; }

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	// member
	float e[3];

	// operator reloading
	inline vec3f&operator += (const vec3f& v);
	inline vec3f&operator -= (const vec3f& v);
	inline vec3f&operator *= (const vec3f& v);
	inline vec3f&operator /= (const vec3f& v);

	inline vec3f&operator *= (const float f);
	inline vec3f&operator /= (const float f);

	inline const vec3f&operator +() const { return *this; }
	inline vec3f operator-() const { return vec3f(-e[0], -e[1], -e[2]); }

	inline float length() const {
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}

};


inline vec3f&vec3f::operator += (const vec3f& v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline vec3f&vec3f::operator-=(const vec3f &v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline vec3f&vec3f::operator*=(const vec3f &v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline vec3f&vec3f::operator/=(const vec3f &v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline vec3f&vec3f::operator*=(const float f) {
	e[0] *= f;
	e[1] *= f;
	e[2] *= f;
	return *this;
}

inline vec3f&vec3f::operator/=(const float f) {
	e[0] /= f;
	e[1] /= f;
	e[2] /= f;
	return *this;
}

inline vec3f operator+(const vec3f& v1, const vec3f& v2) {
	return vec3f(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3f operator-(const vec3f& v1, const vec3f& v2) {
	return vec3f(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3f operator/(const vec3f& v1, const vec3f& v2) {
	return vec3f(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3f operator*(const vec3f& v1, const vec3f& v2) {
	return vec3f(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3f operator*(const vec3f& v, float f) {
	return vec3f(v.e[0] * f, v.e[1] * f, v.e[2] * f);
}

inline vec3f operator/(const vec3f& v, float f) {
	return vec3f(v.e[0] / f, v.e[1] / f, v.e[2] / f);
}

// dot product
inline float dot(const vec3f& v1, const vec3f& v2) {
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

// cross product
inline vec3f cross(const vec3f& v1, const vec3f& v2) {
	return vec3f(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
		v1.e[2]  *v2.e[0] - v1.e[0] * v2.e[2],
		v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

// unit vector
// remove a member function uniform(), since when a const vec3 object call uniform(),
// the compiler would throw an error, because if uniform() is a non-const function,
// calling a non-const function DO NOT guarantee the const object's constness.
inline vec3f unit(const vec3f&v) {
	return vec3f(v.e[0] / v.length(), v.e[1] / v.length(), v.e[2] / v.length());
}

inline std::istream& operator >> (std::istream& is, vec3f& v) {
	is >> v.e[0] >> v.e[1] >> v.e[2];
	return is;
}

inline std::ostream& operator<<(std::ostream& os, vec3f& v) {
	os << v.e[0] << " " << v.e[1] << " " << v.e[2];
	return os;
}

inline void colorClamp(vec3f &v) {
	if (v.e[0] > 1.0f || v.e[1] > 1.0f || v.e[2] > 1.0f) {
		v /= 255.0f;
	}
	return;
}

#endif //BART_RAY_TRACER_VEC3F_H
