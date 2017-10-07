#ifndef BART_RAY_TRACER_CORE_H
#define BART_RAY_TRACER_CORE_H

#include "vec3f.h"

#define _USE_MATH_DEFINES
#include <math.h>

typedef float Vec2f[2];
typedef float Vec3f[3];
typedef float Vec4f[4];

#define G_MAX_DEPTH 0	// bouncing count
#define G_BVH_DEPTH	1
#define G_PLANE_SET_NORMAL_NUM 7

#endif
