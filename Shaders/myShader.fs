#version 330 core
out vec4 FragColor;
in vec2 oTextureCoord;

in vec3 oPos; // object's position in local space
in vec3 oPosInWorldSpace; // object's position in world space
in vec3 oVertexNormal;

uniform vec3 objectColor;
uniform float lightAngle;
uniform sampler2D texture1;

void main()
{
	vec3 lightPos = vec3(0.0f, 0.0f, 0.0f);
	// lightPos.x = cos(lightAngle) / 2.0f;
	// lightPos.y = sin(lightAngle) / 2.0f;
	// lightPos.z = 0.0;

	vec3 vertexNormal = normalize(oVertexNormal);
	vec3 lightDirection = normalize(lightPos - oPosInWorldSpace);
	float lightIntense = dot(lightDirection, vertexNormal);
	vec3 finalColor = objectColor * lightIntense;

	//FragColor = vec4(vertexNormal, 1.0f);
	FragColor = lightIntense * texture(texture1, oTextureCoord); // output color and alpha
}