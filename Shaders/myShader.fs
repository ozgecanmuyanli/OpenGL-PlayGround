#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;
in vec3 oPosInWorldSpace; // object's position in world space
in vec3 oVertexNormal;

uniform sampler2D texture1;

void main()
{
	vec3 lightPos = vec3(0.0f, 0.0f, 5.0f);

	vec3 vertexNormal = normalize(oVertexNormal);
	vec3 lightDirection = normalize(lightPos - oPosInWorldSpace);
	float lightIntense = dot(lightDirection, vertexNormal);

	FragColor = vec4(vec3(lightIntense), 1.0f); 
}