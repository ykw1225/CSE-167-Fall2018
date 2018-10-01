#include "Window.h"

const char* window_title = "GLFW Starter Project";

// Cubic Obj
Cube cube(5.0f);

// Objects
vector<OBJObject*> Window::objects;

// pointer to current display obj
OBJObject* currObj;

int Window::width;
int Window::height;

void Window::addObj(string filePath) {
	objects.push_back(new OBJObject(filePath));
}

void Window::initialize_objects(){
	addObj("bunny.obj");
	addObj("dragon.obj");
	addObj("bear.obj");
	currObj = objects[0];
}

void Window::clean_up(){
	for (auto i : objects) delete i;
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;

	// Set the viewport size
	glViewport(0, 0, width, height);

	// Set the matrix mode to GL_PROJECTION to determine the proper camera properties
	glMatrixMode(GL_PROJECTION);

	// Load the identity matrix
	glLoadIdentity();

	// Set the perspective of the projection viewing frustum
	gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0);

	// Move camera back 20 units so that it looks at the origin (or else it's in the origin)
	glTranslatef(0, 0, -20);
}

void Window::idle_callback()
{
	// Perform any updates as necessary. Here, we will spin the cube slightly.
	//cube.update();
	currObj->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the matrix mode to GL_MODELVIEW
	glMatrixMode(GL_MODELVIEW);

	// Load the identity matrix
	glLoadIdentity();
	
	// Render objects
	//cube.draw();
	currObj->draw();

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
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
}
