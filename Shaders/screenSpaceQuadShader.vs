#version 330 core

out vec2 oTextureCoord;

void main()
{
    vec3 vertices[6] =vec3 [] 
                        (vec3(-1.0f, 0.7f, 0.0f),
                        vec3(-0.6f, 0.7f, 0.0f),
                        vec3(-0.6f, 1.0f, 0.0f),
                        vec3(-0.6f, 1.0f, 0.0f),
                        vec3(-1.0f, 1.0f, 0.0f),
                        vec3(-1.0f, 0.7f, 0.0f));
    
    vec2 texVertices[6] = vec2 []
                            (vec2(0.0f, 0.0f),
                            vec2(1.0f, 0.0f),
                            vec2(1.0f, 1.0f),
                            vec2(1.0f, 1.0f),
                            vec2(0.0f, 1.0f),
                            vec2(0.0f, 0.0f));

    oTextureCoord = vec2(texVertices[gl_VertexID]);            
    gl_Position = vec4(vertices[gl_VertexID], 1.0f);
}