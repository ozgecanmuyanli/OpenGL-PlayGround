#version 330 core
layout (location=0) in vec3 aPos; // position attribute

out vec3 oPos;

void main()
{
	oPos = aPos;
	gl_Position = vec4(aPos, 1.0f); // position and w
}