#ifndef _ROBOT_H_
#define _ROBOT_H_

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
#include "MT.h"
#include "Geo.h"

using namespace std;
using namespace glm;

const string BODY = "body_s.obj", HEAD = "head_s.obj", LIMB = "limb_s.obj", EYE = "eyeball_s.obj", ANT = "antenna_s.obj";

class Robot {
private:
	Geo *headGeo, *leftArmGeo, *rightArmGeo, 
		*bodyGeo, *leftLegGeo, *rightLegGeo, 
		*leftEyeGeo, *rightEyeGeo, *leftAntGeo, *rightAntGeo;
	MT *modelMtx, *headMtx, *leftArmMtx, *rightArmMtx, 
		*bodyMtx, *leftLegMtx, *rightLegMtx, 
		*leftEyeMtx, *rightEyeMtx, *leftAntMtx, *rightAntMtx;

public:
	Robot() {
		headGeo = new Geo(HEAD); bodyGeo = new Geo(BODY);
		leftArmGeo = new Geo(LIMB); rightArmGeo = new Geo(LIMB);
		leftLegGeo = new Geo(LIMB); rightLegGeo = new Geo(LIMB);
		leftEyeGeo = new Geo(EYE); rightEyeGeo = new Geo(EYE);
		leftAntGeo = new Geo(ANT); rightAntGeo = new Geo(ANT);

		modelMtx = new MT(mat4(1.0f), vec3(0.0f), vec3(0.0f));
		headMtx = new MT(mat4(1.0f), vec3(0.0f, 0.1f, 0.0f), vec3(0.0f));
		bodyMtx = new MT(translate(mat4(1.0f), vec3(0.0f, -1.0f, 0.0f)), 
			vec3(0.0f), vec3(0.0f));

		leftArmMtx = new MT(scale(rotate(translate(mat4(1.0f), vec3(-1.3f, -0.9f, 0.0f)),
			15.0f / 180.0f * pi<float>(), vec3(0.0f, 0.0f, -30.0f)),
			vec3(1.5f, 1.5f, 1.5f)), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.5f, 0.0f));

		rightArmMtx = new MT(scale(rotate(translate(mat4(1.0f), vec3(+1.3f, -0.9f, 0.0f)),
			15.0f / 180.0f * pi<float>(), vec3(0.0f, 0.0f, 30.0f)),
			vec3(1.5f, 1.5f, 1.5f)), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.5f, 0.0f));

		leftLegMtx = new MT(scale(translate(mat4(1.0f), vec3(-0.6f, -2.5f, 0.0f)),
			vec3(1.5f, 1.5f, 1.5f)), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.5f, 0.0f));

		rightLegMtx = new MT(scale(translate(mat4(1.0f), vec3(+0.6f, -2.5f, 0.0f)),
			vec3(1.5f, 1.5f, 1.5f)), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.5f, 0.0f));

		leftEyeMtx = new MT(scale(translate(mat4(1.0f), vec3(-0.2f, 0.5f, 0.8f)),
			vec3(1.2f, 1.2f, 1.2f)), -vec3(1.0f, 1.0f, 1.0f), vec3(0.0f));

		rightEyeMtx = new MT(scale(translate(mat4(1.0f), vec3(+0.2f, 0.5f, 0.8f)),
			vec3(1.2f, 1.2f, 1.2f)), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f));

		leftAntMtx = new MT(scale(rotate(translate(mat4(1.0f), vec3(-0.5f, 0.5f, 0.0f)),
			15.0f / 180.0f * pi<float>(), vec3(0.0f, 0.0f, 30.0f)),
			vec3(0.5f, 0.5f, 0.5f)), vec3(0.0f), vec3(0.0f));

		rightAntMtx = new MT(scale(rotate(translate(mat4(1.0f), vec3(+0.5f, 0.5f, 0.0f)),
			15.0f / 180.0f * pi<float>(), vec3(0.0f, 0.0f, -30.0f)),
			vec3(0.5f, 0.5f, 0.5f)), vec3(0.0f), vec3(0.0f));

		modelMtx->addChild(headMtx); modelMtx->addChild(bodyMtx);
		modelMtx->addChild(leftArmMtx); modelMtx->addChild(rightArmMtx);
		modelMtx->addChild(leftLegMtx); modelMtx->addChild(rightLegMtx);
		modelMtx->addChild(leftEyeMtx); modelMtx->addChild(rightEyeMtx);
		modelMtx->addChild(leftAntMtx); modelMtx->addChild(rightAntMtx);

		headMtx->addChild(headGeo); bodyMtx->addChild(bodyGeo);
		leftArmMtx->addChild(leftArmGeo); rightArmMtx->addChild(rightArmGeo);
		leftLegMtx->addChild(leftLegGeo); rightLegMtx->addChild(rightLegGeo);
		leftEyeMtx->addChild(leftEyeGeo); rightEyeMtx->addChild(rightEyeGeo);
		leftAntMtx->addChild(leftAntGeo); rightAntMtx->addChild(rightAntGeo);
	}
	~Robot() {
		delete headGeo, bodyGeo, leftArmGeo, rightArmGeo, 
			leftLegGeo, rightLegGeo, leftEyeGeo, rightEyeGeo,
			leftAntGeo, rightAntGeo;
		delete modelMtx, headMtx, bodyMtx, 
			leftArmMtx, rightArmMtx, leftLegMtx, rightLegMtx, 
			leftEyeMtx, rightEyeMtx, leftAntMtx, rightAntMtx;
	}
	void draw(mat4 C, GLuint shaderProgram) { modelMtx->draw(C, shaderProgram); }
	void update() { 
		headMtx->update();
		leftArmMtx->update();
		rightArmMtx->update();
		leftLegMtx->update();
		rightLegMtx->update();
		leftEyeMtx->update();
		rightEyeMtx->update();
	}
};

#endif