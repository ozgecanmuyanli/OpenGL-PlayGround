#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;

uniform sampler2D mapTexture;

void main()
{
	FragColor = texture(mapTexture, oTextureCoord); 
}