#ifndef _SPHERE_H
#define _SPHERE_H

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

struct Combb {
	vec3 vertex;
	vec3 normal;
	vec3 color;
	Combb(vec3 v, vec3 n, vec3 c) {
		vertex = v;
		normal = n;
		color = c;
	}
};

class Sphere {
private:
	vector<unsigned int> indices;
	vector<vec3> vertices, normals, colors;
	GLuint VBO, VAO, EBO;
	vector<Combb> combs;

	void setupPipeline();

public:
	mat4 toWorld;
	unsigned int textureID;


	Sphere();
	void parse(string);
	void draw(GLuint);
	void move(vec3);
};

#endif
