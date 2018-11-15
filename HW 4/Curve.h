#ifndef CURVE_h
#define CURVE_h

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


class Curve{
    public:
		Curve();
		~Curve();
		void setup();
		void draw(GLuint);
		vec3 getPos();

		vector<vec3> ctrlPoints, points;
		vector<int> indices;
		int pointsNum, seg, currIndex;
		vec3 currPos;
		GLuint VBO, VAO, EBO;
};

#endif
