#include "window.h"

const char* window_title = "GLFW Starter Project";
// Cube * cube;
GLint shaderProgram;

// Objects
vector<OBJObject*> Window::objects;

// pointer to current display obj
OBJObject* currObj;

// tracker of current point size
GLfloat Window::currPoint;

// tracker of current mouse position
vec3 Window::mouseCurPos;

// the initial mouse position
vec3 Window::mouseInitPos;

// tracker if the left and right click of mouse
bool Window::leftClick = false;
bool Window::rightClick = false;

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
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::addObj(string filePath) {
	objects.push_back(new OBJObject(filePath));
}

void Window::initialize_objects() {
	// cube = new Cube();

	string bunny = "bunny.obj", dragon = "dragon.obj", bear = "bear.obj";
	#ifdef __APPLE__
		bunny = "/Users/KIT/Desktop/CSE 167/CSE 167/bunny.obj";
		dragon = "/Users/KIT/Desktop/CSE 167/CSE 167/dragon.obj";
		bear = "/Users/KIT/Desktop/CSE 167/CSE 167/bear.obj";
	#endif
	addObj(bunny);
	addObj(dragon);
	addObj(bear);
	currObj = objects[0];

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up() {
	// delete(cube);

	for (auto i : objects) delete i;
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height) {
	// Initialize GLFW
	if (!glfwInit())
	{
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
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

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
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height) {
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback() {
	// Call the update function the cube
	// cube->update();
	currObj->update();
}

void Window::display_callback(GLFWwindow* window) {
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	
	// Render the cube
	// cube->draw(shaderProgram);
	currObj->draw(shaderProgram);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Check for a key press
	if (action == GLFW_PRESS) {
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
	else if (key == GLFW_KEY_F1) currObj = objects[0];
	else if (key == GLFW_KEY_F2) currObj = objects[1];
	else if (key == GLFW_KEY_F3) currObj = objects[2];
	else if (key == GLFW_KEY_P) {
		if (mods == GLFW_MOD_SHIFT) {
			glGetFloatv(GL_POINT_SIZE, &currPoint);
			glPointSize(++currPoint);
		}
		else if (mods != GLFW_MOD_SHIFT) {
			glGetFloatv(GL_POINT_SIZE, &currPoint);
			glPointSize(--currPoint);
		}
		if (currPoint >= 1.0f) currObj->point = currPoint;
	}
	else if (key == GLFW_KEY_S) {
		if (mods == GLFW_MOD_SHIFT) { for (auto i : objects) i->scaleUp(); }
		else if (mods != GLFW_MOD_SHIFT) { for (auto i : objects) i->scaleDown(); }
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double x, double y) {
	mouseCurPos = vec3((float)x, (float)y, 0.0f);
	auto last = toTrackBall(mouseInitPos);
	auto curr = toTrackBall(mouseCurPos);

	if (leftClick) leftClicking(last, curr);

	mouseInitPos = mouseCurPos;
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	glfwGetCursorPos(window, &xCursor, &yCursor);
	mouseInitPos = vec3((float)xCursor, (float)yCursor, 0.0f);

	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) leftClick = true;
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) rightClick = true;
	}
	else if (action == GLFW_RELEASE) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) leftClick = false;
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) rightClick = false;
	}

}

vec3 Window::toTrackBall(vec3 v) {
	float w = (float)Window::width, h = (float)Window::height;
	float x = -(2.0f * v.x - w) / w;
	float y = (2.0f * v.y - h) / h;
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
		cam_pos = mat3(rotate(mat4(1.0f), rotAngle / 180.0f * pi<float>(), rotAxis)) * cam_pos;
		V = lookAt(cam_pos, cam_look_at, cam_up);
	}

}