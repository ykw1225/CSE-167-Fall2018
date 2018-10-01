#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;
using namespace glm;

class OBJObject {
private:
	vector<unsigned int> indices;
	vector<vec3> vertices;
	vector<vec3> normals;
	vector<vec3> colors;
	mat4 toWorld;

public:
	OBJObject(string filepath);
	void parse(string filepath);
	void draw();
};

#endif