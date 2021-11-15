#version 330 core
out vec4 FragColor;

in vec3 oPosInWorldSpace;
in vec3 oVertexNormal;
in vec2 oTextureCoord;

uniform sampler2D textureWood;
uniform vec3 viewPos;

void main()
{
    vec3 lightPos = vec3(1.0f, 1.0f, 1.0f);

	vec3 vertexNormal = normalize(oVertexNormal);
	vec3 lightDirection = normalize(lightPos - oPosInWorldSpace);
	float lightIntense = dot(lightDirection, vertexNormal);

	float specularStrength = 1.5f;
	vec3 viewDir = normalize(viewPos - oPosInWorldSpace);
	vec3 reflectDir = reflect(-lightDirection, vertexNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * texture(textureWood, oTextureCoord).rgb;

    FragColor = vec4((vec3(lightIntense) + specular), 1.0f) * texture(textureWood, oTextureCoord);
}