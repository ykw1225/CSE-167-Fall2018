#ifndef _ARMY_H_
#define _ARMY_H_

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
#include "Robot.h"
#include "Sphere.h"

using namespace std;
using namespace glm;

class Army {
private:
	mat4 M = mat4(1.0f);

public:
	vector<Robot*> army;
	vector<Sphere*> spheres;

	Army() {
		for (int i = -5; i < 5; i++) {
			for (int j = -5; j < 5; j++) {
				army.push_back(new Robot(translate(M, 2.7f * vec3((float)i, 0.0f, (float)j))));
				spheres.push_back(new Sphere("sphere.obj", translate(M, vec3(2.0f * (float)i, 0.0f, 2.0f * (float)j))));
			}
		}

	}
	~Army() { for (auto i : army) delete i; for (auto i : spheres) delete i;}

	void draw(mat4 C, GLuint shader, GLuint shaderSphere) { 
		for (auto i : army) i->draw(C, shader); 
		for (auto i : spheres) i->draw(scale(translate(mat4(1.0f),
			vec3(0.0f, -0.8f, 0.0f)), vec3(2.7f, 2.7f, 2.7f)), shaderSphere);
	}
	void update() { for (auto i : army) i->update(); }
};

#endif