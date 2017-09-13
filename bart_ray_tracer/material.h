//
// Created by yang on 17-8-9.
//

#ifndef BART_RAY_TRACER_MATERIAL_H
#define BART_RAY_TRACER_MATERIAL_H

#include "vec3f.h"

class Material {
public:
    vec3f ambient;
    vec3f diffuse;
    vec3f specular;
    vec3f reflect;
    float shine;
    float T;
    float ior;

    Material() : ambient(), diffuse(), specular(), reflect(0.1), shine(0), T(1), ior(0) {}

    inline void setAmbient(vec3f amb) { ambient = amb; }
    inline void setDiffuse(vec3f diff) { diffuse = diff; }
    inline void setSpecular(vec3f spc) { specular = spc; }
    inline void setReflect(vec3f ref) { reflect = ref; }
};

#endif //BART_RAY_TRACER_MATERIAL_H
