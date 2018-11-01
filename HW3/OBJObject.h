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

struct Comb {
	vec3 vertex;
	vec2 texCoor;
	Comb(vec3 v, vec2 t) {
		vertex = v;
		texCoor = t;
	}
};

class OBJObject {
private:
	vector<unsigned int> indices;
	vector<vec3> vertices, normals, colors;
	vector<vec2> texCoors;
	GLuint VBO, VAO, EBO, textureID;;
	vector<Comb> combs;

	void parse(string);
	void setupPipeline();
	
public:
	mat4 toWorld;

	OBJObject(string);
	void draw(mat4, GLuint);
	void update();
};

#endif
