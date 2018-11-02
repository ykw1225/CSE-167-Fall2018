#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "shader.h"

#include "Army.h"

using namespace std;
using namespace glm;

class Window {
public:
	static int width;
	static int height;
	static mat4 P; // P for projection
	static mat4 V; // V for view

	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	////////////////////////////////////////////////////////////////////////////////////////////////

	static vec3 mouseCurPos, mouseInitPos;
	static bool leftClick, rightClick;
	static double xCursor, yCursor;

	static void cursor_position_callback(GLFWwindow*, double, double);
	static void mouse_button_callback(GLFWwindow*, int, int, int);
	static vec3 toTrackBall(vec3);
	static void leftClicking(vec3, vec3);
	static void cursor_scroll_callback(GLFWwindow* window, double x, double y);
};

#endif
