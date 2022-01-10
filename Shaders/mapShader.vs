#version 330 core

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec2 oTextureCoord;

vec3 gridVertices[6] = vec3[] (
    vec3(-1,0,1), vec3(1,0,1), vec3(1,0,-1),
    vec3(1,0,-1), vec3(-1,0,-1), vec3(-1,0,1)
);

void main()
{
    vec3 pos = gridVertices[gl_VertexID].xyz;
    oTextureCoord.x = (pos.x + 1);
    oTextureCoord.y = (-pos.z + 1);
    
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0);
}