#ifndef _GEO_H_
#define _GEO_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Node.h"
#include "OBJObject.h"

using namespace std;
using namespace glm;

class Geo :public Node {
private:
	mat4 modelview;
	OBJObject* obj;

public:
	Geo(string filepath, int i = 0) { obj = new OBJObject(filepath, i); }
	~Geo() { delete obj; }
	void draw(mat4 C, GLuint shader) {
		modelview = C;
		obj->draw(C, shader);
	}
	void update() { obj->update(); }
};

#endif
