#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTextureCoord;
layout(location = 1) in vec3 aVertexNormal;


uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec2 oTextureCoord;
out vec3 oVertexNormal;
out vec4 oEyeSpacePos;

void main()
{
    mat4 mv = view * model;
    mat4 mvp = projection * mv;

    oTextureCoord = aTextureCoord;
    oVertexNormal = aVertexNormal;
    oEyeSpacePos = mv * vec4(aPos, 1.0f);

    gl_Position = mvp * vec4(aPos, 1.0f);
}