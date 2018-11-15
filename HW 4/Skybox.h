#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

using namespace std;
using namespace glm;

const int V_SIZE = 108, F_SIZE = 6;
const float SIDE = 1000.0f;

class Skybox{
private:
	GLfloat vertices[V_SIZE] = {
		-SIDE,  SIDE, -SIDE,
		-SIDE, -SIDE, -SIDE,
		SIDE, -SIDE, -SIDE,
		SIDE, -SIDE, -SIDE,
		SIDE,  SIDE, -SIDE,
		-SIDE,  SIDE, -SIDE,

		-SIDE, -SIDE,  SIDE,
		-SIDE, -SIDE, -SIDE,
		-SIDE,  SIDE, -SIDE,
		-SIDE,  SIDE, -SIDE,
		-SIDE,  SIDE,  SIDE,
		-SIDE, -SIDE,  SIDE,

		SIDE, -SIDE, -SIDE,
		SIDE, -SIDE,  SIDE,
		SIDE,  SIDE,  SIDE,
		SIDE,  SIDE,  SIDE,
		SIDE,  SIDE, -SIDE,
		SIDE, -SIDE, -SIDE,

		-SIDE, -SIDE,  SIDE,
		-SIDE,  SIDE,  SIDE,
		SIDE,  SIDE,  SIDE,
		SIDE,  SIDE,  SIDE,
		SIDE, -SIDE,  SIDE,
		-SIDE, -SIDE,  SIDE,

		-SIDE,  SIDE, -SIDE,
		SIDE,  SIDE, -SIDE,
		SIDE,  SIDE,  SIDE,
		SIDE,  SIDE,  SIDE,
		-SIDE,  SIDE,  SIDE,
		-SIDE,  SIDE, -SIDE,

		-SIDE, -SIDE, -SIDE,
		-SIDE, -SIDE,  SIDE,
		SIDE, -SIDE, -SIDE,
		SIDE, -SIDE, -SIDE,
		-SIDE, -SIDE,  SIDE,
		SIDE, -SIDE,  SIDE
	};

	void setupVAO();
	void loadTextures();

public:
	unsigned int textureID;
	GLuint skyboxVBO, skyboxVAO, skyboxEBO;
	mat4 toWorld;

	Skybox() { setupVAO(); loadTextures(); }
	void draw(GLint);
};

#endif

