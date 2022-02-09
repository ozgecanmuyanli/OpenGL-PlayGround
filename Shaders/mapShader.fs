#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;
in vec3 oPosInWorldSpace;

uniform sampler2D mapTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 oVertexNormal = vec3(0.0f, 1.0f, 0.0f);

vec3 AddLight()
{
	float specularStrength = 0.5;
	float ambientStrength = 0.25;
	float distance = length(lightPos - oPosInWorldSpace);
    float attenuation = 1.0 / (1.0 + 0.22 * distance + 0.20 * (distance * distance));

	vec3 normalMapValue = normalize(oVertexNormal);
	vec3 pLightDir = normalize(oPosInWorldSpace - lightPos);
	
	vec3 reversedlightDir = -normalize(pLightDir);
	float diffuse = max(dot(normalMapValue,reversedlightDir),0.0) * attenuation;
	vec3 diffuseFactor = diffuse * texture(mapTexture,oTextureCoord).rgb;
	
	vec3 reflectDir = reflect(normalize(pLightDir), normalMapValue);
	vec3 viewDir = normalize(viewPos - oPosInWorldSpace);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 8) *attenuation;
	vec3 specularFactor = vec3(specular * specularStrength);

	vec3 ambientFactor = ambientStrength * texture(mapTexture,oTextureCoord).rgb;

	return diffuseFactor + specularFactor + ambientFactor;
}

void main()
{
	FragColor = vec4(texture(mapTexture,oTextureCoord).rgb, 1.0f); 
}