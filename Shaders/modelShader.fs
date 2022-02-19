#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;
in vec3 oVertexNormal;
in vec3 oPosInWorldSpace;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentWorldPos;
in vec4 FragPosLightSpace;
in mat3 oTBN;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_height1;
uniform sampler2D texture_normal1;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform sampler2D shadowMap;

float CalculateShadow(vec4 fragPosLightSpace)
{
	vec3 normal = texture(texture_normal1, oTextureCoord).rgb;
	float bias = max(0.05 * (1.0 - dot(normal, TangentLightPos)), 0.005);
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// get closest depth value from light’s perspective (using
	// [0,1] range fragPosLight as coords)
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	// get depth of current fragment from light’s perspective
	float currentDepth = projCoords.z;
	// check whether current frag pos is in shadow
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
		
	if(projCoords.z > 1.0)
	{
		shadow = 0.0;
		return shadow;
	}

	return shadow;
}

vec3 AddLight(vec4 fragPosLightSpace) // SHADOW MAPPING - directional light
{
	vec3 normal = oTBN * oVertexNormal ;
	normal = normal * 2.0 - 1.0;

	vec3 pLightDir = normalize(TangentWorldPos - TangentLightPos);
	vec3 reversedlightDir = -normalize(pLightDir);

	float diffuse = max(dot(normal, reversedlightDir), 0.0);
	vec3 diffuseFactor = vec3(diffuse) ;//* texture(texture_diffuse1,oTextureCoord).rgb;
	
	vec3 reflectDir = reflect(normalize(pLightDir), normal);
	vec3 viewDir = normalize(TangentViewPos - TangentWorldPos);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specularFactor = vec3(specular) ;//* texture(texture_specular1, oTextureCoord).rrr;

	float ambientStrength = 0.25;
	vec3 ambientFactor = vec3(ambientStrength) ;//* texture(texture_diffuse1,oTextureCoord).rgb;

	float shadow = CalculateShadow(fragPosLightSpace);
	return (diffuseFactor + specularFactor) * (ambientFactor + (1.0 - shadow));
	//return vec3(1 - shadow);
}



void main()
{
	//FragColor = vec4(AddLight(FragPosLightSpace), 1.0f);
	FragColor = vec4(vec3(0.1f, 0.1f,1.0f) * oVertexNormal, 1.0f);
}