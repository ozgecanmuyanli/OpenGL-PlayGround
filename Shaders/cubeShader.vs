#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTextureCoord;
layout (location=2) in vec3 aVertexNormal;

out vec3 oVertexNormal;
out vec3 oPosInWorldSpace;
out vec2 oTextureCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    oTextureCoord = aTextureCoord * 5.0f;
    oVertexNormal = -aVertexNormal;

    oPosInWorldSpace = vec3(vec4(modelMatrix * vec4(aPos, 1.0f)).xyz);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f);
}