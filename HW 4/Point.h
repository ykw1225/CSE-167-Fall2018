#ifndef _POINT_h
#define _POINT_h

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using namespace std;
using namespace glm;

class Point {
private:
	mat4 toWorld;
	GLuint VBO, VAO;

	void setupPipelinePt();

public:
	Point(vec3);
	void drawPts(GLuint, GLuint);
	vec3 move(vec3);

	vector<vec3> vertPts;
};

#endif