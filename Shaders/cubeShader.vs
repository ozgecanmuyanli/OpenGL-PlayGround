#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTextureCoord;
layout (location=2) in vec3 aVertexNormal;

out vec3 oPos;
out vec2 oTextureCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	oPos = aPos;
	oTextureCoord = aTextureCoord;
	gl_Position = projection * view * model * vec4(oPos, 1.0f);	
}