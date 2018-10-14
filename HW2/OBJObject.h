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
#include <cstdlib>
#include <ctime>
#include <cfloat>

using namespace std;
using namespace glm;

struct Comb {
	vec3 vertex;
	vec3 normal;
	vec3 color;
	Comb(vec3 v, vec3 n, vec3 c) {
		vertex = v;
		normal = n;
		color = c;
	}
};

class OBJObject {
private:
	mat4 toWorld;
	vector<unsigned int> indices;
	vector<vec3> vertices, normals, colors, randVer, altColors;
	float angle, xMin, xMax, yMin, yMax, zMin, zMax, speed;
	GLuint VBO, VAO, EBO;
	vector<Comb> combs;

	void spin(float);
	void minMax(float, float, float);
	void centerAndScale();
	float randFloat(float, float);
	void verMove();
	void floatChange(float&, float);
	void setupPipeline();
	
public:
	float point;
	bool alterColor;
	OBJObject(string filepath);
	void parse(string filepath);
	void draw(GLuint);
	void update();
	void scaleUp() { toWorld = scale(toWorld, vec3(1.2f, 1.2f, 1.2f)); }
	void scaleDown() { toWorld = scale(toWorld, vec3(0.8f, 0.8f, 0.8f)); }
};

#endif
