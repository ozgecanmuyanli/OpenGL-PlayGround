#version 330 core
out vec4 FragColor;

in vec3 oPosInWorldSpace;
in vec3 oVertexNormal;
in vec2 oTextureCoord;

uniform sampler2D textureWood;
uniform vec3 viewPos;

vec3 AddLight()
{
	vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);;
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);//
    float ambientStrength = 0.1;
    float specularStrength = 0.5f;

    // Ambient
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
	vec3 vertexNormal = normalize(oVertexNormal);
	vec3 lightDirection = normalize(lightPos - oPosInWorldSpace);
    float diff = max(dot(vertexNormal, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
	vec3 viewDir = normalize(viewPos - oPosInWorldSpace);
    vec3 halfwayDir = normalize(lightDirection + viewDir);
    float spec = pow(max(dot(vertexNormal, halfwayDir), 0.0), 32.0);

	vec3 specular = spec * lightColor;
    vec3 phongLight = (ambient + diffuse + specular) * texture(textureWood, oTextureCoord).rgb;
    return phongLight;
}

void main()
{
    vec3 phongLight = AddLight();
    FragColor = vec4(phongLight, 1.0);
}