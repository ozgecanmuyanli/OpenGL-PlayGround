#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aLineColor;

out vec3 oColor;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    vec3 vertices[6] =vec3 [] 
                        (vec3(0.0f, 0.0f, 0.0f), vec3(999.0f, 0.0f, 0.0f),
                        vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 999.0f, 0.0f),
                        vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 999.0f));
                        
    oColor = vertices[gl_VertexID];
    gl_Position = projectionMatrix * viewMatrix * vec4(vertices[gl_VertexID], 1.0f);
}