#version 330 core
layout (location=0) in vec3 aPos; // position attribute
layout (location=1) in vec2 aTextureCoord;
layout (location=2) in vec3 aVertexNormal;

out vec3 oPos;
out vec3 oPosInWorldSpace;
out vec2 oTextureCoord;
out vec3 oVertexNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	oVertexNormal = aVertexNormal;
	oTextureCoord = aTextureCoord;
	oPos = aPos;
	oPosInWorldSpace = vec3(vec4(modelMatrix * vec4(aPos, 1.0f)).xyz);
	//normal vectors should not be affected from translate. So w is not 1, it is set to 0.
	oVertexNormal = vec3(vec4(modelMatrix * vec4(oVertexNormal, 0.0f)).xyz); 
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0f); // position and w
}