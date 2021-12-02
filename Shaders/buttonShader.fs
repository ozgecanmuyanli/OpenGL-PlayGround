#version 330 core
out vec4 FragColor;

in vec2 oTextCoord;

uniform vec3 color;
uniform sampler2D buttonTexture;

void main()
{
    vec3 textColor = texture(buttonTexture, oTextCoord).rgb;
    FragColor = vec4(textColor * color, 1.0f);
}