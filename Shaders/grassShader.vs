#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTextureCoord;


out vec2 oTextureCoord;
out vec4 oPosInWorldSpace;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float time;

void main()
{
    oTextureCoord = aTextureCoord;
    vec4 oPosInWorldSpace = vec4(modelMatrix * vec4(aPos, 1.0f));
    if (aPos.y > 0.0)
    {
        oPosInWorldSpace.x = oPosInWorldSpace.x + sin(time);
    }
    gl_Position = projectionMatrix * viewMatrix * oPosInWorldSpace;
}