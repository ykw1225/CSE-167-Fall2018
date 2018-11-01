#ifndef MT_H_
#define MT_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Node.h"

using namespace std;
using namespace glm;

class MT :public Node {
private:
	mat4 M;
	vector<Node*> children;

public:
	MT(mat4 m) : M(m) {}
	void addChild(Node* n) { children.push_back(n); }
	void draw(mat4 C, GLuint shaderProgram) { for (auto i : children) i->draw(C * M, shaderProgram); }
	void update() { for (auto i : children) i->update(); }
};

#endif
