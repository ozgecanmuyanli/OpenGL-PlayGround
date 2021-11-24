#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;
in vec4 oPosInWorldSpace;

uniform sampler2D textureGrass;

void main()
{
    vec4 texColor = texture(textureGrass, oTextureCoord);
    if (texColor.a < 0.1)
    {
        discard;
    }
    FragColor = texColor;
}