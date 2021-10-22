#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTextureCoord;

out vec3 oPos;
out vec2 oTextureCoord;

uniform mat4 modelMatrix;

void main()
{
	oPos = aPos;
	oTextureCoord = aTextureCoord;
	gl_Position = modelMatrix * vec4(aPos, 1.0);
}