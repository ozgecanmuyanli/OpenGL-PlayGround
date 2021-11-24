#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTextureCoord;


out vec2 oTextureCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main()
{
    oTextureCoord = aTextureCoord;
    //vec4 oPosInWorldSpace = vec4(modelMatrix * vec4(aPos, 1.0f));
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
}