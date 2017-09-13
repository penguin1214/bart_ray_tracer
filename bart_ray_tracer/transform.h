//
// Created by yang on 17-8-6.
//

#ifndef BART_RAY_TRACER_TRANSFORM_H
#define BART_RAY_TRACER_TRANSFORM_H

#include <vector>
#include "matrix4x4.h"

class TransformHierarchy {
public:
    bool is_static;
    Matrix4x4 transformMatrix;
    std::vector<TransformHierarchy*> children;

    TransformHierarchy(bool is_s) : is_static(is_s) {}
};

#endif //BART_RAY_TRACER_TRANSFORM_H
