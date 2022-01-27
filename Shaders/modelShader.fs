#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;
in vec3 oVertexNormal;
in vec3 oPosInWorldSpace;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentWorldPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_height1;
uniform sampler2D texture_normal1;
uniform vec3 viewPos;
uniform vec3 lightPos;

vec3 AddLight() // NORMAL MAPPING
{
	vec3 normal = texture(texture_normal1, oTextureCoord).rgb;
	normal = normal * 2.0 - 1.0;

	vec3 pLightDir = normalize(TangentLightPos - TangentWorldPos);
	float diffuse = max(dot(normal, pLightDir), 0.0);
	vec3 diffuseFactor = diffuse * texture(texture_diffuse1,oTextureCoord).rgb;
	
	vec3 reflectDir = reflect(-normalize(pLightDir), normal);
	vec3 viewDir = normalize(TangentViewPos - TangentWorldPos);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specularFactor = specular * texture(texture_specular1, oTextureCoord).rrr;

	float ambientStrength = 0.25;
	vec3 ambientFactor = ambientStrength * texture(texture_diffuse1,oTextureCoord).rgb;

	return diffuseFactor + specularFactor + ambientFactor;
}

void main()
{
	FragColor = vec4(AddLight(), 1.0f);
}