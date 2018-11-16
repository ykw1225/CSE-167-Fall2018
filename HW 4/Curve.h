#ifndef _CURVE_h
#define _CURVE_h

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

class Curve {
private:
	mat4x3 m;
	
	vec4 calCT(float t);
	void setupPipeline();
	void setupPipelinePt();
	void findVert(vec3, vec3);

public:
	mat4 toWorld;
	GLuint VBO, VAO, VCO, VDO;
	vector<vec3> controlPoints, vertQ, vertPts;

	Curve() {}
	Curve(vec3, vec3, vec3, vec3);
	void draw(GLuint);
	void update(GLuint, vec3);
};
#endif
