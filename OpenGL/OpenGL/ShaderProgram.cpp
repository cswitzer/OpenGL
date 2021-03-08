#include "ShaderProgram.h"
#include <fstream>  // for reading in files
#include <iostream> // output information to the console
#include <sstream>  // makes converting files to strings easy

ShaderProgram::ShaderProgram()
	: mHandle(0)
{
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(mHandle);
}

/***************************************************
  amazing code that keeps me from having to write
  the same code over and over again!
****************************************************/
bool ShaderProgram::loadShaders(const char* vsFileName, const char* fsFileName)
{
	// convert files to strings
	string vsString = fileToString(vsFileName);
	string fsString = fileToString(fsFileName);

	// we need char pointers for the glShaderSrc function
	const GLchar *vsSourcePtr = vsString.c_str();
	const GLchar *fsSourcePtr = fsString.c_str();

	// create shaders
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	// retrieve shader sources
	glShaderSource(vs, 1, &vsSourcePtr, NULL);
	glShaderSource(fs, 1, &fsSourcePtr, NULL);

	// compile the shaders
	glCompileShader(vs);
	glCompileShader(fs);

	// check for compiler errors
	checkCompileErrors(vs, VERTEX);
	checkCompileErrors(fs, FRAGMENT);

	// create the program and attach shaders
	mHandle = glCreateProgram();
	glAttachShader(mHandle, vs);
	glAttachShader(mHandle, fs);

	// finally, link the program!
	glLinkProgram(mHandle);

	// check to see if linking the program had issues
	checkCompileErrors(mHandle, PROGRAM);

	// we already linked the shaders to the program, so we don't need them anymore
	glDeleteShader(vs);
	glDeleteShader(fs);
	return true;
}

void ShaderProgram::use()
{
	// if there is a shader program, use it
	if (mHandle > 0)
		glUseProgram(mHandle);
}

/***************************************************
  load shaders from files, not hardcoded strings
****************************************************/
string ShaderProgram::fileToString(const string& filename)
{
	std::stringstream ss; // makes a string object a stream. Makes a string object like cout or cin
	std::ifstream file;

	try
	{
		file.open(filename, std::ios::in);

		// if the file successfully loaded, load its contents into the string stream
		if (!file.fail())
		{
			ss << file.rdbuf();
		}

		file.close();
	}
	catch (std::exception ex)
	{
		std::cerr << "Error reading shader filename!" << std::endl;
	}

	return ss.str();
}

/***************************************************
  checks to see if shaders compiles correctly
  or if the shader program linked correctly
****************************************************/
void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type)
{
	int status = 0;

	// are we linking our program or compiling our shaders?
	if (type == PROGRAM)
	{
		glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			// get the length of the error
			glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);

			// construct a string of the specified length and initialize it with all spaces
			string errorLog(length, ' ');
			glGetProgramInfoLog(mHandle, length, &length, &errorLog[0] /*pointer to first char in string*/);

			std::cerr << "Error! Program failed to link. " << errorLog << std::endl;
		}
	}
	else // VERTEX or FRAGMENT
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			// get the length of the error
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			// construct a string of the specified length and initialize it with all spaces
			string errorLog(length, ' ');
			glGetShaderInfoLog(shader, length, &length, &errorLog[0] /*pointer to first char in string*/);

			std::cerr << "Error! Shader failed to compile. " << errorLog << std::endl;
		}
	}
}