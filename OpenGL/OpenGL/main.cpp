#include <iostream>
#include <sstream>

#define GLEW_STATIC
#include "GL/glew.h";
#include "GLFW/glfw3.h";

#include "ShaderProgram.h"

// GLEW must come before GLFW

const char* APP_TITLE = "Introduction to Modern OpenGL - Hello Shader";
const float gWindowWidth = 800;
const float gWindowHeight = 600;
GLFWwindow* gWindow = NULL;
bool gFullScreen = false;

using namespace std;

// key pressing function must be like this exactly (the function can be named anything though)
// This function will be called everytime a key is hit
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void showFPS(GLFWwindow* window);
bool initOpenGL();

int main()
{
	// if openGL fails to initialize, exit the program
	if (!initOpenGL())
	{
		std::cerr << "GFLW initialization failed" << std::endl;
		return -1;
	}

	// rendering a triangle
	GLfloat vertices[] = {
		// position         // color
		 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // top
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f// left
	};

	// vertex buffer objects are generic places in memory (blobs in memory) that hold data
	// send the vertices to the GPU right away
	GLuint vbo, vao;

	/*
	VAO's keep track of vertex buffer objects that are bound, the offset in there
	address spaces, the stride between the vertex attributes, and how to interpret
	underlying data
	*/
	
	// Vertex array object generation
	glGenVertexArrays(1, &vao);
	// create 1 buffer that creates a blob of memory for us
	glGenBuffers(1, &vbo);

	// this is the VAO we want to draw from
	glBindVertexArray(vao);

	// make vbo the current buffer. OpenGL only allows one buffer to be active at a time
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// fill the buffer with data the size of our "vertices" array
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
	* We need to tell the GPU what all our information above means
	* Vertex Attrib Pointer tells attribute pointers in VAO where to reference currently bound buffer's attributes (color, position)
	*/

	// VAO attrib pointer points to POSITION in our VBO here
	glVertexAttribPointer(
		0,                   /*same as '0' in layout (location = 0)*/
		3,                   /*3 elements*/
		GL_FLOAT,			 /*float is the datatype*/
		GL_FALSE,  
		sizeof(GL_FLOAT) * 6,/*stride*/ 
		NULL                 /*offset starts at beginning*/
	);
	// Enable drawing from 0th position in the VAO
	glEnableVertexAttribArray(0);

	// VAO attrib pointer points to COLOR in our VBO here
	glVertexAttribPointer(
		1,                             /*same as '1' in layout (location = 1)*/
		3,                             /*3 elements*/
		GL_FLOAT,                      /*float is the datatype*/
		GL_FALSE, 
		sizeof(GL_FLOAT) * 6,          /*stride*/
		(GLvoid*)(sizeof(GLfloat)* 3)  /*color attribute starts at position 3*/
	);
	// Enable drawing from 0th position in the VAO
	glEnableVertexAttribArray(1);

	// create shader program using our shader class
	ShaderProgram shaderProgram;
	shaderProgram.loadShaders("shaders/basic.vert", "shaders/basic.frag");


	// stay in this loop until the window should be closed
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);

		// poll for keyboard and mouse inputs
		glfwPollEvents();

		// clear the screen every frame so we can draw on a clean canvas
		glClear(GL_COLOR_BUFFER_BIT);

		// use the shaderProgram
		shaderProgram.use();

		// we want to draw from this vertex array object
		glBindVertexArray(vao);

		// start from the 0th position in our vertex array (start from positions in this case). 3 indicates that there are 3 vertices
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// unselect the vertex array object after it finishes drawing
		glBindVertexArray(0);

		// Allows front and back buffer to swap. This leads to smooth buffer transitions (avoids flicker)
		glfwSwapBuffers(gWindow);
	}

	// clean things up before we are done
	
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();
	return 0;
}

/*
 This function returns false if initialization fails in any away
*/
bool initOpenGL()
{
	if (!glfwInit())
	{
		// if glfw can't initialize, then we have no window
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	// create a window in OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// We want to force the application to use only the newest version of OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (gFullScreen) {
		// reference the monitor we are using right now
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();

		// get the specs of our monitor
		const GLFWvidmode* pVmode = glfwGetVideoMode(pMonitor);

		// if a monitor is found, use that monitor
		if (pVmode != NULL) {
			gWindow = glfwCreateWindow(pVmode->width, pVmode->height, APP_TITLE, pMonitor, NULL);
		}
	}
	else {
		gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	}

	// if the wind fails to be created, exit immediately
	if (gWindow == NULL)
	{
		cerr << "Failed to create GLFW window" << endl;
		// make sure to terminate glfw as well
		glfwTerminate();
		return false;
	}

	// make our current window the window we draw to! 
	glfwMakeContextCurrent(gWindow);

	// callback functions that handle user input
	glfwSetKeyCallback(gWindow, glfw_onKey);

	// glew queries our video card and finds all the function pointers to all the functions we need
	glewExperimental = GL_TRUE; // mandatory for new versions of modern OpenGL
	// exit if GLEW fails to initialize
	if (glewInit() != GLEW_OK)
	{
		cerr << "GLEW failed to initialize" << endl;
		return false;
	}

	// openGL is a state machine. It will store the state of the clear color.
	// This means we don't need to call it in the while loop every frame
	glClearColor(0.23f, 0.38f, 0.47f, 1.0f); // the color we will clean the canvas with

	// everything is ok at this point
	return true;
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

	// first iteration: .25 - 0, second iteration: .50 - .25
	elapsedSeconds = currentSeconds - previousSeconds;

	// limit text update 4 times per second
	if (elapsedSeconds > 0.25)
	{
		// in first case, previous seconds = .25
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