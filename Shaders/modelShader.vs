#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aVertexNormal;
layout (location=2) in vec2 aTextureCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 oTextureCoord;
out vec3 oVertexNormal;
out vec3 oPosInWorldSpace;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentWorldPos;
out vec4 FragPosLightSpace;
out mat3 oTBN;
out vec4 oEyeSpacePos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform mat4 lightSpaceMatrix;

void main()
{
	mat4 mv = view * model;
	oEyeSpacePos = mv * vec4(aPos, 1.0f);
	oVertexNormal = aVertexNormal;
	oTextureCoord = aTextureCoord;
	//oTextureCoord.y = 1.0 - oTextureCoord.y; // for backpack model textures
	oPosInWorldSpace = (model * vec4(aPos, 1.0f)).xyz;

	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 N = normalize(normalMatrix * aVertexNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N));
	oTBN = TBN;
	TangentLightPos = TBN * lightPos;
	TangentViewPos = TBN * viewPos;
	TangentWorldPos = TBN * oPosInWorldSpace;

	FragPosLightSpace = lightSpaceMatrix * vec4(oPosInWorldSpace, 1.0);

	gl_Position = projection * view * model * vec4(aPos, 1.0f);	
}