#include "window.h"

const char* window_title = "GLFW Starter Project";
// Cube * cube;
GLint shaderSkybox, shaderCurve, shaderPt, shaderLine, shaderProgram;

// tracker of current mouse position
vec3 Window::mouseCurPos;

// the initial mouse position
vec3 Window::mouseInitPos;

// tracker if the left and right click of mouse
bool Window::leftClick = false;
bool Window::rightClick = false;
bool Window::pointClick = false;
bool Window::go = false;
bool Window::FPP = false;

// tracker of the x and y value of cursor
double Window::xCursor;
double Window::yCursor;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"
#ifdef __APPLE__
	#define VERTEX_SHADER_PATH "/Users/KIT/Desktop/CSE 167/CSE 167/shader.vert"
	#define FRAGMENT_SHADER_PATH "/Users/KIT/Desktop/CSE 167/CSE 167/shader.frag"
#endif

// Default camera parameters
vec3 Window::cam_pos(0.0f, 0.0f, 10.0f);		// e  | Position of camera
vec3 Window::cam_look_at(0.0f, 0.0f, 0.0f);		// d  | This is where the camera looks at
vec3 Window::cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

mat4 Window::P;
mat4 Window::V;

Skybox *skybox;
Sphere *sphere;
Track  *track;

vec3 Window::camPos;
vec3 Window::camLookAt;
vec3 Window::camUp(0.0f, 1.0f, 0.0f);

int curPt = 0, index;

void Window::reCalCam() {
	camPos = track->vertices[index + 2];
	camLookAt = track->vertices[(index + 6) % track->vertices.size()];
	vec3 dir = normalize(camLookAt - camPos);
	camPos -= dir * 1.1f;
	camUp = cross(cross(dir, vec3(0.0f, 1.0f, 0.0f)), dir);
	camPos += camUp * 1.2f;
}

void Window::initialize_objects() {
	skybox = new Skybox();
	sphere = new Sphere();
	track = new Track();
	sphere->textureID = skybox->textureID;
	sphere->toWorld = translate(mat4(1.0f), track->vertices[0]);

	index = 0;
	reCalCam();

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	shaderSkybox = LoadShaders("../skybox.vert", "../skybox.frag");
	shaderCurve = LoadShaders("../curveShader.vert", "../curveShader.frag");
	shaderPt = LoadShaders("../ptShader.vert", "../ptShader.frag");
	shaderLine = LoadShaders("../lineShader.vert", "../lineShader.frag");
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up() {
	delete skybox, sphere, track;
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderSkybox);
	glDeleteProgram(shaderCurve);
	glDeleteProgram(shaderPt);
	glDeleteProgram(shaderLine);
}

GLFWwindow* Window::create_window(int w, int h) {
	// Initialize GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(w, h, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &w, &h);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, w, h);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int w, int h) {
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	width = w;
	height = h;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, w, h);

	if (height > 0) {
		P = perspective(radians(45.0f), (float)w / (float)h, 0.1f, 1000.0f);
		if(FPP) V = lookAt(camPos, camLookAt, camUp);
		else V = lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback() {
	if (index == track->vertices.size()) index = 0;
	if (Window::go) index++;
	// Call the update function the cube
	if (go) {
		sphere->move(track->vertices[index + 0]);
		reCalCam();
	}
	if (FPP) V = lookAt(camPos, camLookAt, camUp);
	else V = lookAt(cam_pos, cam_look_at, cam_up);
}

void Window::display_callback(GLFWwindow* window) {
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox->draw(shaderSkybox);
	track->draw(shaderCurve);
	if (pointClick) {
		track->drawpoint(shaderPt);
		track->drawLine(shaderLine);
	}
	sphere->draw(shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	vec3 forwardDir = normalize(cam_look_at - cam_pos);
	float speed = 0.5f;

	// Check for a key press
	if (action == GLFW_PRESS) {
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE) {
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_C) pointClick = !pointClick;
		if (key == GLFW_KEY_P) go = !go;
		if (key == GLFW_KEY_F) FPP = !FPP;
	}
	if (!FPP) {
		if (key == GLFW_KEY_W) {
			cam_pos += forwardDir * speed;
			cam_look_at += forwardDir * speed;
		}
		if (key == GLFW_KEY_S) {
			cam_pos -= forwardDir * speed;
			cam_look_at -= forwardDir * speed;
		}
	}
	if (pointClick) {
		float s = 0.1f;
		if (key == GLFW_KEY_X) {
			vec3 v;
			if (mods == GLFW_MOD_SHIFT) v = vec3(1.0f, 0.0f, 0.0f);
			else v = vec3(-1.0f, 0.0f, 0.0f);
			pointControling(v * s);
		}
		if (key == GLFW_KEY_Y) {
			vec3 v;
			if (mods == GLFW_MOD_SHIFT) v = vec3(0.0f, 1.0f, 0.0f);
			else v = vec3(0.0f, -1.0f, 0.0f);
			pointControling(v * s);
		}
		if (key == GLFW_KEY_Z) {
			vec3 v;
			if (mods == GLFW_MOD_SHIFT) v = vec3(0.0f, 0.0f, 1.0f);
			else v = vec3(0.0f, 0.0f, -1.0f);
			pointControling(v * s);
		}
		sphere->move(track->vertices[index + 0]);
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double x, double y) {
	mouseCurPos = vec3((float)x, (float)y, 0.0f);
	auto last = toTrackBall(mouseInitPos);
	auto curr = toTrackBall(mouseCurPos);

	if (leftClick && !FPP && !pointClick) leftClicking(last, curr);

	mouseInitPos = mouseCurPos;
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	glfwGetCursorPos(window, &xCursor, &yCursor);
	mouseInitPos = vec3((float)xCursor, (float)yCursor, 0.0f);

	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			leftClick = true;
			if (pointClick) {
				curPt+=3;
				if (curPt == 24) curPt = 0;
			}
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			rightClick = true;
			if (pointClick) {
				curPt-=3;
				if (curPt == -3) curPt = 21;
			}
		}
		cout << curPt << endl;
	}
	else if (action == GLFW_RELEASE) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) leftClick = false;
		if (button == GLFW_MOUSE_BUTTON_RIGHT) rightClick = false;
	}
}

vec3 Window::toTrackBall(vec3 v) {
	float w = (float)width, h = (float)height;
	float x = (2.0f * v.x - w) / w;
	float y = -(2.0f * v.y - h) / h;
	auto vNew = vec3(x, y, 0.0f);
	auto d = length(vNew);
	d = d < 1.0f ? d : 1.0f;
	vNew.z = sqrtf(1.001f - d * d);
	vNew = normalize(vNew);
	return vNew;
}

void Window::leftClicking(vec3 last, vec3 curr) {
	auto direction = (curr - last) * 10.0f;
	auto velocity = length(direction);
	if (velocity > 0.0001f) {
		auto rotAxis = cross(last, curr);
		auto rotAngle = velocity * 10.0f;
		if (rotAngle > 360.0f || rotAngle < -360.0f) rotAngle = 0.0f;
		cam_pos = mat3(rotate(mat4(1.0f), (rotAngle / 180.0f * pi<float>()), rotAxis)) * cam_pos;
		vec3 dir = cam_look_at - cam_pos;
		cam_up = cross(cross(dir, vec3(0.0f, 1.0f, 0.0f)), dir);
	}
}

void Window::pointControling(vec3 v) {
	vec3 temp = track->pts[curPt]->move(v * 2.0f);
	track->pts[(curPt + 1) % 24]->move(v);
	track->pts[(curPt + 23) % 24]->move(v);
	track->update(curPt / 3, 1, temp);
	if (curPt != 0) track->update((curPt / 3 + 23) % 24, 4, temp);
	else track->update(7, 4, temp);

	temp = track->pts[(curPt + 23) % 24]->move(v);
	track->update((curPt + 23) % 24 / 3, 3, temp);
	temp = track->pts[(curPt + 1) % 24]->move(v);
	track->update((curPt + 1) / 3, 2, temp);
}