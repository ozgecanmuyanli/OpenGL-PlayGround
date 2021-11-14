#version 330 core
layout (location=0) in vec3 aPos;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform sampler2D textureNoise;
uniform float time;

out vec2 oTextureCoord;
out vec3 oPos;
out float finalHeight;

void main()
{
    oPos = aPos;
	oTextureCoord = aPos.xz;
    oTextureCoord.x = oTextureCoord.x + (time * 0.2);
    float vertexHeight = texture(textureNoise, oTextureCoord).r * 255.0;
    finalHeight = (vertexHeight - 100.0) / 4000.0; //normalization

    oPos.y = (-finalHeight * 6 + 6);
    
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(oPos, 1.0);
}