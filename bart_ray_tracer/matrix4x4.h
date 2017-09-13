//
// Created by yang on 17-8-6.
//

#ifndef BART_RAY_TRACER_MATRIX4X4_H
#define BART_RAY_TRACER_MATRIX4X4_H

#include <cstring>

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
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				m[i][j] = this->m[i][0] * mat.m[0][j] + this->m[i][1] * mat.m[1][j] + this->m[i][2] * mat.m[2][j] + this->m[i][3] * mat.m[3][j];
			}
		}
		return *this;
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

inline Matrix4x4 rotate(float x, float y, float z, float degree)
{
	Matrix4x4 m;
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
