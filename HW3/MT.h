#ifndef _MT_H_
#define _MT_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include "Node.h"

using namespace std;
using namespace glm;

class MT :public Node {
private:
	mat4 M;
	vec3 rotateVec, transtVec;
	vector<Node*> children;
	float delta = 0.7f;
	int counter = 0;

public:
	MT(mat4 m, vec3 r, vec3 t){
		M = m;
		rotateVec = r;
		transtVec = t;
	}
	void addChild(Node* n) { children.push_back(n); }
	void draw(mat4 C, GLuint shaderProgram) { for (auto i : children) i->draw(C * M, shaderProgram); }
	void update() { 
		if (counter < 100) {
			M = translate(M, transtVec);
			M = rotate(M, delta / 180.0f * pi<float>(), rotateVec);
			M = translate(M, -transtVec);
			counter++;
		}
		else {
			rotateVec = -rotateVec;
			counter = -counter;
		}
		for (auto i : children) i->update();
	}
};

#endif
