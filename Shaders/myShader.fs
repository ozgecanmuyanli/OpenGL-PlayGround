#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;
in vec3 oPosInWorldSpace; // object's position in world space
in vec3 oVertexNormal;
in vec3 oPos;
in float finalHeight;

uniform sampler2D texture1;
uniform vec3 viewPos;

void main()
{
	//vec3 lightPos = vec3(oPosInWorldSpace.x, oPosInWorldSpace.y + 3.0f, oPosInWorldSpace.z);
	vec3 lightPos = vec3(30.0f, 30.0f, 30.0f);

	vec3 vertexNormal = normalize(oVertexNormal);
	vec3 lightDirection = normalize(lightPos - oPosInWorldSpace);
	float lightIntense = dot(lightDirection, vertexNormal);

	float specularStrength = 1.5f;
	vec3 viewDir = normalize(viewPos - oPosInWorldSpace);
	vec3 reflectDir = reflect(-lightDirection, vertexNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * texture(texture1, oTextureCoord).rgb;

	vec3 finalColor;
	if(finalHeight > 0.62) //height treshold
	{
		finalColor = vec3(255.0 / 255.0, 110.0 / 255.0, 110.0 / 255.0);
	}
	else if(finalHeight > 0.60)
	{
		finalColor = vec3(255.0 / 255.0 , 255.0 / 255.0, 36.0 / 255.0);
	}
	else 
	{
		finalColor = vec3(128.0 / 255.0, 255.0 / 255.0, 128.0 / 255.0);
	}
	
	FragColor = vec4(vec3(finalColor * texture(texture1, oTextureCoord).bbb), 1.0f); 
}