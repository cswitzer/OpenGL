#version 330 core
layout (location = 0) in vec3 pos; // vector 3 for our VBO's position attributes
layout (location = 1) in vec3 color;

out vec3 vert_color;

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
	vert_color = color;
};