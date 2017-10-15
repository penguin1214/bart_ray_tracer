//
// Created by yang on 17-8-6.
//

#ifndef BART_RAY_TRACER_TRANSFORM_H
#define BART_RAY_TRACER_TRANSFORM_H

#include <vector>
#include <memory>
#include <string>
#include <stack>
#include "matrix4x4.h"

class Mesh;

class TransformHierarchy {
public:
    bool _is_static;
	std::string _name;
    Matrix4x4 _transformMatrix;
    TransformHierarchy *_parent;
    Mesh *_mesh;
    //std::vector<TransformHierarchy*> _children;
	TransformHierarchy *_child;

	TransformHierarchy(bool is_s) : _is_static(is_s) {
		_transformMatrix = Matrix4x4();
		_parent = nullptr; _mesh = nullptr; _name = "";
	}

	void addChild(TransformHierarchy* t) {
		//_children.push_back(t);
		_child = t;
		t->_parent = this;
	}

	void animate(double trans[3], double rot[4], double scl[3]) {
		Matrix4x4 m;
		m *= translate(trans[0], trans[1], trans[2]);
		m *= rotate(rot[0], rot[1], rot[2], rot[3]);
		m *= scale(scl[0], scl[1], scl[2]);
		_transformMatrix = m;
	}
};

extern std::stack<TransformHierarchy*> transformHierarchy;

#endif //BART_RAY_TRACER_TRANSFORM_H
