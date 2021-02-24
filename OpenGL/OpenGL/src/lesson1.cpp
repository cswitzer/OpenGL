#include <iostream>
#include <sstream>
#define GLEW_STATIC
#include "GL/glew.h";
#include "GLFW/glfw3.h";

// GLEW must come before GLFW

const char* APP_TITLE = "Basic Game";
const float gWindowWidth = 800;
const float gWindowHeight = 600;
bool gFullScreen = false;

using namespace std;

// key pressing function must be like this exactly (the function can be named anything though)
// This function will be called everytime a key is hit
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void showFPS(GLFWwindow* window);

int main()
{
	if (!glfwInit())
	{
		// if glfw can't initialize, then we have no window
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}

	// create a window in OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// We want to force the application to use only the newest version of OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* pWindow = NULL;

	if (gFullScreen) {
		// reference the monitor we are using right now
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();

		// get the specs of our monitor
		const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);

		// if a monitor is found, use that monitor
		if (pVmode != NULL) {
			pWindow = glfwCreateWindow(pVmode->width, pVmode->height, APP_TITLE, pMonitor, NULL);
		}
	}
	else {
		pWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	}

	// if the wind fails to be created, exit immediately
	if (pWindow == NULL)
	{
		cerr << "Failed to create GLFW window" << endl;
		// make sure to terminate glfw as well
		glfwTerminate();
		return -1;
	}

	// make our current window the window we draw to! 
	glfwMakeContextCurrent(pWindow);

	// callback functions that handle user input
	glfwSetKeyCallback(pWindow, glfw_onKey);

	// glew queries our video card and finds all the function pointers to all the functions we need
	glewExperimental = GL_TRUE; // mandatory for new versions of modern OpenGL
	// exit if GLEW fails to initialize
	if (glewInit() != GLEW_OK)
	{
		cerr << "GLEW failed to initialize" << endl;
		return -1;
	}

	// stay in this loop until the window should be closed
	while (!glfwWindowShouldClose(pWindow))
	{
		showFPS(pWindow);

		// poll for keyboard and mouse inputs
		glfwPollEvents();

		// clear the screen every frame so we can draw on a clean canvas
		glClearColor(0.23f, 0.38f, 0.47f, 1.0f); // the color we will clean the canvas with
		glClear(GL_COLOR_BUFFER_BIT);

		// Allows front and back buffer to swap. This leads to smooth buffer transitions (avoids flicker)
		glfwSwapBuffers(pWindow);
	}

	glfwTerminate();
	return 0;
}

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// bailing out of the while loop in main if the escape key is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void showFPS(GLFWwindow* window)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started as a double

	elapsedSeconds = currentSeconds - previousSeconds;

	// limit text update 4 times per second
	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		// the frame count per second
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		// output frames per second to the screen
		std::ostringstream outs;
		outs.precision(3);
		// append to after our title
		outs << std::fixed << APP_TITLE << "   "
			<< "FPS: " << fps << "   " << "Frame Time: "
			<< msPerFrame << " (ms)";
		glfwSetWindowTitle(window, outs.str().c_str());

		frameCount = 0;
	}
	frameCount++;
}