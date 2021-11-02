#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;
in vec3 oPosInWorldSpace; // object's position in world space
in vec3 oVertexNormal;
in vec3 oPos;

uniform sampler2D texture1;

void main()
{
	//vec3 lightPos = vec3(oPosInWorldSpace.x, oPosInWorldSpace.y + 3.0f, oPosInWorldSpace.z);
	vec3 lightPos = vec3(5.0f, 5.0f, 5.0f);

	vec3 vertexNormal = normalize(oVertexNormal);
	vec3 lightDirection = normalize(lightPos - oPosInWorldSpace);
	float lightIntense = dot(lightDirection, vertexNormal);

	FragColor = vec4(vec3(lightIntense), 1.0f) * texture(texture1, oTextureCoord); 
}