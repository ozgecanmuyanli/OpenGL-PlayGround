#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;

uniform vec3 color;
uniform sampler2D entityTexture;

void main()
{
	FragColor = texture(entityTexture, oTextureCoord);
}