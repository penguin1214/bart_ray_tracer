//
// Created by yang on 17-8-4.
//

#ifndef BART_RAY_TRACER_MESH_H
#define BART_RAY_TRACER_MESH_H

#include <vector>
#include "vec3f.h"

class Triangle;

class Mesh {
public:
    uint32_t nverts;
    vec3f *verts;
    Material material;  // do not use pointer here because of the copy issue
    std::vector<Triangle* > triangles;

    Mesh(uint32_t nv, vec3f *v) {
        nverts = nv; verts = v;
    }

    void addTriangle(Triangle* tri) {triangles.push_back(tri);}
};

#endif //BART_RAY_TRACER_MESH_H
