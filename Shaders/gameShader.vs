#version 330 core
layout (location=0) in vec3 aPos;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform sampler2D mapTexture;

out vec2 oTextureCoord;
out vec3 oPos;

void main()
{
    oPos = aPos;
	oTextureCoord = aPos.xz;
    
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(oPos, 1.0);
}