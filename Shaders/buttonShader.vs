#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTextCoord;

out vec2 oTextCoord;

uniform mat4 projection;
uniform mat4 model;

void main()
{
    oTextCoord = aTextCoord;
    gl_Position = projection * model * vec4(aPos, 1.0f);
}