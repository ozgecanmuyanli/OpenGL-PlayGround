#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;

uniform sampler2D textureWindow;

void main()
{
    vec4 texColor = texture(textureWindow, oTextureCoord);
    FragColor = texColor;
}