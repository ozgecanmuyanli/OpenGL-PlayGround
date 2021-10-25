#version 330 core
layout (location=0) in vec3 aPos; // position attribute

out vec3 oPos;
out vec3 oPosInWorldSpace;

uniform mat4 modelMatrix;

void main()
{
	oPos = aPos;
	oPosInWorldSpace = vec3(vec4(modelMatrix * vec4(aPos, 1.0f)).xyz); 
	gl_Position = modelMatrix * vec4(aPos, 1.0f); // position and w
}