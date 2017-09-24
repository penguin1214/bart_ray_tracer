//
// Created by yang on 17-8-6.
//

#ifndef BART_RAY_TRACER_MATRIX4X4_H
#define BART_RAY_TRACER_MATRIX4X4_H

#include <cstring>
#include "vec3f.h"

class Matrix4x4
{
  public:
	float m[4][4];

	Matrix4x4()
	{
		//      *this = identity(); // is this OK?
		m[0][0] = 1.0;
		m[0][1] = 0.0;
		m[0][2] = 0.0;
		m[0][3] = 0.0;
		m[1][0] = 0.0;
		m[1][1] = 1.0;
		m[1][2] = 0.0;
		m[1][3] = 0.0;
		m[2][0] = 0.0;
		m[2][1] = 0.0;
		m[2][2] = 1.0;
		m[2][3] = 0.0;
		m[3][0] = 0.0;
		m[3][1] = 0.0;
		m[3][2] = 0.0;
		m[3][3] = 1.0;
	}
	Matrix4x4(float mat[4][4])
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m[i][j] = mat[i][j];
			}
		}
	}
	Matrix4x4(float *mat) { memcpy(m, mat, sizeof(float) * 16); }
	Matrix4x4(const Matrix4x4 &mat) { memcpy(m, mat.m, sizeof(float) * 16); }

	float *operator[](unsigned int i) { return m[i]; }
	Matrix4x4 &operator*=(const Matrix4x4 mat)
	{
		Matrix4x4 tmp = *this;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				m[i][j] = tmp[i][0] * mat.m[0][j]
					+ tmp[i][1] * mat.m[1][j]
					+ tmp[i][2] * mat.m[2][j]
					+ tmp[i][3] * mat.m[3][j];
			}
		}
		return *this;
	}

	
	vec3f operator*(vec3f &v) {
		vec3f ret;
		ret.e[0] = v.x()*m[0][0] + v.y()*m[0][1] + v.z()*m[0][2] + m[0][3];
		ret.e[1] = v.x()*m[1][0] + v.y()*m[1][1] + v.z()*m[1][2] + m[1][3];
		ret.e[2] = v.x()*m[2][0] + v.y()*m[2][1] + v.z()*m[2][2] + m[2][3];
		return ret;
	}

	Matrix4x4 operator* (Matrix4x4 &mat) {
		Matrix4x4 ret;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				ret[i][j] = m[i][0] * mat[0][j] + m[i][1] * mat[1][j]
					+ m[i][2] * mat[2][j] + m[i][3] * mat[3][j];
			}
		}
		return ret;
	}

	Matrix4x4 identity();
	float determinate();
	Matrix4x4 inverse();
};

inline Matrix4x4 Matrix4x4::identity()
{
	float mat[4][4];
	mat[0][0] = 1.0;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = 0.0;
	mat[1][0] = 0.0;
	mat[1][1] = 1.0;
	mat[1][2] = 0.0;
	mat[1][3] = 0.0;
	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 1.0;
	mat[2][3] = 0.0;
	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;

	return Matrix4x4(mat);
}

inline float det3(float a, float b, float c, float d, float e, float f, float g, float h, float i)
{
	return a * e * i + d * h * c + g * b * f - g * e * c - d * b * i - a * h * f;
}

inline float Matrix4x4::determinate()
{
	float det;
	det = m[0][0] * det3(m[1][1], m[1][2], m[1][3],
						 m[2][1], m[2][2], m[2][3],
						 m[3][1], m[3][2], m[3][3]);

	det -= m[0][1] * det3(m[1][0], m[1][2], m[1][3],
						  m[2][0], m[2][2], m[2][3],
						  m[3][0], m[3][2], m[3][3]);

	det += m[0][2] * det3(m[1][0], m[1][1], m[1][3],
						  m[2][0], m[2][1], m[2][3],
						  m[3][0], m[3][1], m[3][3]);

	det -= m[0][3] * det3(m[1][0], m[1][1], m[1][2],
						  m[2][0], m[2][1], m[2][2],
						  m[3][0], m[3][1], m[3][2]);

	return det;
}

inline Matrix4x4 Matrix4x4::inverse()
{
	Matrix4x4 inv;
	float det = determinate();
	inv.m[0][0] = det3(m[1][1], m[1][2], m[1][3],
							 m[2][1], m[2][2], m[2][3],
							 m[3][1], m[3][2], m[3][3]) /
						det;
	inv.m[0][1] = -det3(m[0][1], m[0][2], m[0][3],
							  m[2][1], m[2][2], m[2][3],
							  m[3][1], m[3][2], m[3][3]) /
						det;
	inv.m[0][2] = det3(m[0][1], m[0][2], m[0][3],
							 m[1][1], m[1][2], m[1][3],
							 m[3][1], m[3][2], m[3][3]) /
						det;
	inv.m[0][3] = -det3(m[0][1], m[0][2], m[0][3],
							  m[1][1], m[1][2], m[1][3],
							  m[2][1], m[2][2], m[2][3]) /
						det;

	inv.m[1][0] = -det3(m[1][0], m[1][2], m[1][3],
							  m[2][0], m[2][2], m[2][3],
							  m[3][0], m[3][2], m[3][3]) /
						det;
	inv.m[1][1] = det3(m[0][0], m[0][2], m[0][3],
							 m[2][0], m[2][2], m[2][3],
							 m[3][0], m[3][2], m[3][3]) /
						det;
	inv.m[1][2] = -det3(m[0][0], m[0][2], m[0][3],
							  m[1][0], m[1][2], m[1][3],
							  m[3][0], m[3][2], m[3][3]) /
						det;
	inv.m[1][3] = det3(m[0][0], m[0][2], m[0][3],
							 m[1][0], m[1][2], m[1][3],
							 m[2][0], m[2][2], m[2][3]) /
						det;

	inv.m[2][0] = det3(m[1][0], m[1][1], m[1][3],
							 m[2][0], m[2][1], m[2][3],
							 m[3][0], m[3][1], m[3][3]) /
						det;
	inv.m[2][1] = -det3(m[0][0], m[0][1], m[0][3],
							  m[2][0], m[2][1], m[2][3],
							  m[3][0], m[3][1], m[3][3]) /
						det;
	inv.m[2][2] = det3(m[0][0], m[0][1], m[0][3],
							 m[1][0], m[1][1], m[1][3],
							 m[3][0], m[3][1], m[3][3]) /
						det;
	inv.m[2][3] = -det3(m[0][0], m[0][1], m[0][3],
							  m[1][0], m[1][1], m[1][3],
							  m[2][0], m[2][1], m[2][3]) /
						det;

	inv.m[3][0] = -det3(m[1][0], m[1][1], m[1][2],
							  m[2][0], m[2][1], m[2][2],
							  m[3][0], m[3][1], m[3][2]) /
						det;
	inv.m[3][1] = det3(m[0][0], m[0][1], m[0][2],
							 m[2][0], m[2][1], m[2][2],
							 m[3][0], m[3][1], m[3][2]) /
						det;
	inv.m[3][2] = -det3(m[0][0], m[0][1], m[0][2],
							  m[1][0], m[1][1], m[1][2],
							  m[3][0], m[3][1], m[3][2]) /
						det;
	inv.m[3][3] = det3(m[0][0], m[0][1], m[0][2],
							 m[1][0], m[1][1], m[1][2],
							 m[2][0], m[2][1], m[2][2]) /
						det;

	return inv;
}

inline Matrix4x4 translate(float x, float y, float z)
{
	Matrix4x4 m;
	m[0][3] = x;
	m[1][3] = y;
	m[2][3] = z;
	return m;
}

/* TODO: rotate */
inline Matrix4x4 rotate(float x, float y, float z, float degree)
{
	Matrix4x4 m;
	float cos_alpha = cos(degree);
	float sin_alpha = sin(degree);

	if (x != 0.0 && y == 0.0 && z == 0.0) {  // rotate according to x axis
		m[1][1] = cos_alpha;
		m[1][2] = -sin_alpha;
		m[2][1] = sin_alpha;
		m[2][2] = cos_alpha;
	}
	else if (x == 0.0 && y != 0.0 && z == 0.0) { // rotate according to y axis
		m[0][0] = cos_alpha;
		m[0][2] = sin_alpha;
		m[2][0] = -sin_alpha;
		m[2][2] = cos_alpha;
	}
	else if (x == 0.0 && y == 0.0 && z != 0.0) {
		m[0][0] = cos_alpha;
		m[0][1] = -sin_alpha;
		m[1][0] = sin_alpha;
		m[1][1] = cos_alpha;
	}
	else {
		vec3f axis = vec3f(x, y, z);
		axis = unit(axis);
		x = axis.x();
		y = axis.y();
		z = axis.z();

		m[0][0] = (1 - cos_alpha) * x * x + cos_alpha;
		m[0][1] = (1 - cos_alpha) * x * y - sin_alpha * y;
		m[0][2] = (1 - cos_alpha) * x * z + sin_alpha * y;

		m[1][0] = (1 - cos_alpha) * x * y + sin_alpha * z;
		m[1][1] = (1 - cos_alpha) * y * y + cos_alpha;
		m[1][2] = (1 - cos_alpha) * y * z - sin_alpha * x;

		m[2][0] = (1 - cos_alpha) * x * z - sin_alpha * y;
		m[2][1] = (1 - cos_alpha) * y * z + sin_alpha * x;
		m[2][2] = (1 - cos_alpha) * z * z + cos_alpha;
	}
	return m;
}

inline Matrix4x4 scale(float x, float y, float z)
{
	Matrix4x4 m;
	m[0][0] = x;
	m[1][1] = y;
	m[2][2] = z;
	return m;
}

#endif //BART_RAY_TRACER_MATRIX4X4_H
