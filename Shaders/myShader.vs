#version 330 core
layout (location=0) in vec3 aPos; // position attribute
layout (location=1) in vec2 aTextureCoord; // texture position attribute

out vec2 oTextureCoord;
out vec3 oPos;

uniform mat4 modelMatrix;

void main()
{
	oTextureCoord = aTextureCoord;
	oPos = aPos;
	gl_Position = modelMatrix * vec4(aPos, 1.0f); // position and w
}