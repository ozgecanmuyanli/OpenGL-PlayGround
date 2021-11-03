#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;
in vec3 oPosInWorldSpace; // object's position in world space
in vec3 oVertexNormal;
in vec3 oPos;

uniform sampler2D texture1;
uniform vec3 viewPos;

void main()
{
	//vec3 lightPos = vec3(oPosInWorldSpace.x, oPosInWorldSpace.y + 3.0f, oPosInWorldSpace.z);
	vec3 lightPos = vec3(3.0f, 1.0f, 3.0f);

	vec3 vertexNormal = normalize(oVertexNormal);
	vec3 lightDirection = normalize(lightPos - oPosInWorldSpace);
	float lightIntense = dot(lightDirection, vertexNormal);

	float specularStrength = 1.5f;
	vec3 viewDir = normalize(viewPos - oPosInWorldSpace);
	vec3 reflectDir = reflect(-lightDirection, vertexNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * texture(texture1, oTextureCoord).rgb;

	FragColor = vec4((vec3(lightIntense) + specular), 1.0f) * texture(texture1, oTextureCoord); 
}