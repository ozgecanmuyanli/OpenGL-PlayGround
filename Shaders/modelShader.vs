#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTextureCoord;
layout (location=2) in vec3 aVertexNormal;

out vec3 oPosInWorldSpace;
out vec2 oTextureCoord;
out vec3 oVertexNormal;
out vec3 oPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform sampler2D textureModel;
uniform sampler2D textureTerrain;
uniform float modelScaleSize;
uniform float gridScaleSize;

void main()
{
	oPos = aPos;

	vec3 modelCenterPoint = vec3(0.0f ,0.0f ,0.0f);
	vec3 modelCenterPointInWorld = vec3(vec4(modelMatrix * vec4(modelCenterPoint, 1.0f)).xyz);
	modelCenterPointInWorld = modelCenterPointInWorld / gridScaleSize;

	// Terrain height calculation
	oTextureCoord = modelCenterPointInWorld.xz;
	float greenChannelValue = texture(textureTerrain, oTextureCoord).g * 255.0 * 255.0; 
    float vertexHeight = texture(textureTerrain, oTextureCoord).r * 255.0 + greenChannelValue;
    float finalHeight = (vertexHeight - 8000.0) / 21000.0; // real world height range

	oVertexNormal = vec3(vec4(modelMatrix * vec4(aVertexNormal, 0.0f)).xyz); 
	oPosInWorldSpace = vec3(vec4(modelMatrix * vec4(oPos, 1.0f)).xyz);

	oPosInWorldSpace.y = oPosInWorldSpace.y + (finalHeight * 6) + (0.1f * modelScaleSize); //0.1 is model size
	gl_Position = projectionMatrix * viewMatrix * vec4(oPosInWorldSpace, 1.0f);	
}