//
// Created by yang on 17-8-6.
//

#ifndef BART_RAY_TRACER_TRANSFORM_H
#define BART_RAY_TRACER_TRANSFORM_H

#include <vector>
#include "matrix4x4.h"

class Mesh;

class TransformHierarchy {
public:
    bool _is_static;
    Matrix4x4 _transformMatrix;
    TransformHierarchy *_parent;
    Mesh *_mesh;
    std::vector<TransformHierarchy*> _children;

    TransformHierarchy(bool is_s) : _is_static(is_s) {
        _transformMatrix = Matrix4x4();
        _parent = nullptr; _mesh = nullptr;
    }

    void addChild(TransformHierarchy* t) {
        _children.push_back(t);
        t->_parent = this;
    }
};

extern std::stack<TransformHierarchy* >transformHierarchy;

#endif //BART_RAY_TRACER_TRANSFORM_H
