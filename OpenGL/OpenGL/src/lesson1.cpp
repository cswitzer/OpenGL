#include <iostream>

#define GLEW_STATIC
#include "GL/glew.h";
#include "GLFW/glfw3.h";

// GLEW must come before GLFW

const char* APP_TITLE = "Basic Game";
const float gWindowWidth = 800;
const float gWindowHeight = 600;

using namespace std;

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

	GLFWwindow* pWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
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

