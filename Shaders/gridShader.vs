#version 330 core
layout (location=0) in vec3 aPos;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform sampler2D textureTerrain;

out vec2 oTextureCoord;
out vec3 oPos;
out float finalHeight;

void main()
{
    oPos = aPos;
	oTextureCoord = aPos.xz;
	float greenChannelValue = texture(textureTerrain, oTextureCoord).g * 255.0 * 255.0; 
    float vertexHeight = texture(textureTerrain, oTextureCoord).r * 255.0 + greenChannelValue;
    finalHeight = (vertexHeight - 8000.0) / 21000.0; //real world height range
	oPos.y = (finalHeight * 6);
    
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(oPos, 1.0);
}