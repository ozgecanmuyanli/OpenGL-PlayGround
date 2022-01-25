#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;
in vec3 oVertexNormal;
in vec3 oPosInWorldSpace;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_height1;
uniform sampler2D texture_normal1;
uniform vec3 viewPos;

vec3 AddLight()
{
	vec3 lightDir = vec3(0.0f, 1.0f, 1.0f);
	float ambientStrength = 0.25;

	vec3 normalMapValue = normalize(oVertexNormal);
	vec3 pLightDir = normalize(oPosInWorldSpace - lightDir);
	
	vec3 reversedlightDir = -normalize(pLightDir);
	float diffuse = max(dot(normalMapValue,reversedlightDir),0.0);
	vec3 diffuseFactor = diffuse * texture(texture_diffuse1,oTextureCoord).rgb;
	
	vec3 reflectDir = reflect(normalize(pLightDir), normalMapValue);
	vec3 viewDir = normalize(viewPos - oPosInWorldSpace);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specularFactor = specular * texture(texture_specular1, oTextureCoord).rrr;

	vec3 ambientFactor = ambientStrength * texture(texture_diffuse1,oTextureCoord).rgb;

	return diffuseFactor + specularFactor + ambientFactor;
}

void main()
{
	FragColor = vec4(AddLight(), 1.0f);
}