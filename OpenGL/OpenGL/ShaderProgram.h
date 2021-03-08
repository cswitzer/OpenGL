#pragma once

#include "GL/glew.h";
#include <string>
using std::string;

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	enum ShaderType
	{
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	bool loadShaders(const char* vsFileName, const char* fsFileName);
	void use();

private:
	string fileToString(const string& filename);
	void checkCompileErrors(GLuint shader, ShaderType type);
	
	// a handle to our program
	GLuint mHandle;
};

