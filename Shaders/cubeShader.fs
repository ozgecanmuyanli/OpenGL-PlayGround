#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;
in vec3 oVertexNormal;
in vec3 oPosInWorldSpace;

uniform sampler2D entityTexture;
uniform sampler2D normalMap;
uniform vec3 lightPos;
uniform vec3 viewPos;


float near = 0.1f;
float far = 2.0f;
float LinearizeDepth(float depth)
{
	float ndc = depth * 2.0f - 1.0f; // back to NDC
	float linearDepth = (2.0f * near * far) / (far + near - ndc * (far - near));
	return linearDepth;
}

vec3 AddLight()
{
	float specularStrength = 0.5;
	float ambientStrength = 0.25;
	float distance = length(lightPos - oPosInWorldSpace);
    float attenuation = 1.0 / (1.0 + 0.22 * distance + 0.20 * (distance * distance));

	vec3 normalMapValue = normalize(texture(normalMap,oTextureCoord).rgb);
	vec3 pLightDir = normalize(oPosInWorldSpace - lightPos);
	
	vec3 reversedlightDir = -normalize(pLightDir);
	float diffuse = max(dot(normalMapValue,reversedlightDir),0.0) * attenuation;
	vec3 diffuseFactor = diffuse * texture(entityTexture,oTextureCoord).rgb;
	
	vec3 reflectDir = reflect(normalize(pLightDir), normalMapValue);
	vec3 viewDir = normalize(viewPos - oPosInWorldSpace);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 8) *attenuation;
	vec3 specularFactor = vec3(specular * specularStrength);

	vec3 ambientFactor = ambientStrength * texture(entityTexture,oTextureCoord).rgb;

	return diffuseFactor + specularFactor + ambientFactor;
}


void main()
{
	FragColor = vec4(AddLight(), 1.0f);
}