#version 330 core
out vec4 FragColor;

in vec3 oPos;
uniform vec3 objectColor;
uniform float lightAngle;

void main()
{
	vec3 lightPos;
	lightPos.x = cos(lightAngle) / 2.0f;
	lightPos.y = sin(lightAngle) / 2.0f;
	lightPos.z = 0.25f;

	vec3 vertexNormal = vec3(0.0f, 0.0f, 1.0f); // object's normal vec
	vec3 lightDirection = normalize(lightPos - oPos);
	float lightIntense = dot(lightDirection, vertexNormal);
	vec3 finalColor = objectColor * lightIntense;

	FragColor = vec4(finalColor, 1.0f); // output color and alpha
}