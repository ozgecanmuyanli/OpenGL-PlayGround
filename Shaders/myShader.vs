#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTextureCoord;
layout (location=2) in vec3 aVertexNormal;

out vec3 oPosInWorldSpace;
out vec2 oTextureCoord;
out vec3 oVertexNormal;
out vec3 oPos;
out float finalHeight;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform sampler2D texture1;

void main()
{
	oPos = aPos;
	oTextureCoord = aPos.xz;
	float greenChannelValue = texture(texture1, oTextureCoord).g * 255.0 * 255.0; 
    float vertexHeight = texture(texture1, oTextureCoord).r * 255.0 + greenChannelValue;
    finalHeight = (vertexHeight - 8000.0) / 21000.0; //real world height range
	oPos.y = (finalHeight * 6) - 3.70;

	oVertexNormal = vec3(vec4(modelMatrix * vec4(aVertexNormal, 0.0f)).xyz); 
	oPosInWorldSpace = vec3(vec4(modelMatrix * vec4(oPos, 1.0f)).xyz);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(oPos, 1.0);	
}