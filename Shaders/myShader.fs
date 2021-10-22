#version 330 core
out vec4 FragColor;
in vec3 oPos;
in vec2 oTextureCoord;

//uniform float time;
uniform vec3 objectColor;
uniform float angle;
uniform sampler2D texture1;

void main()
{
	vec3 lightPos;
	lightPos.x = cos(angle) / 2.0f;
	lightPos.y = sin(angle) / 2.0f;
	lightPos.z = 0.25f;

	vec3 vertexNormal = vec3(0.0f, 0.0f, 1.0f);
	vec3 lightDirection = normalize(lightPos - oPos);
    vec3 finalColor;
	float lightIntense = dot(lightDirection, vertexNormal);
	finalColor = objectColor * lightIntense;
	//finalColor.x = finalColor.y * time;
	//FragColor = vec4(finalColor, 1.0f);
	FragColor = lightIntense * texture(texture1, oTextureCoord);
}