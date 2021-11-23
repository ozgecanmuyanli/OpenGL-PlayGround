#version 330 core
out vec4 FragColor;

in vec3 oPosInWorldSpace;
in vec3 oVertexNormal;
in vec2 oTextureCoord;

uniform sampler2D textureWood;
uniform sampler2D textureWindow;
uniform vec3 viewPos;

vec3 AddLight()
{
	vec3 lightPos = vec3(4.5f, 3.5f, 4.5f);
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);//
    float ambientStrength = 0.1;
    float specularStrength = 0.6f;

    //Attenuation
    float distance = length(lightPos - oPosInWorldSpace);
    float attenuation = 1.0 / (1.0f + 0.09 * distance + 0.032 * (distance * distance));

    // Ambient
    vec3 ambient = ambientStrength * lightColor;
    ambient *= attenuation;

    // Diffuse
	vec3 vertexNormal = normalize(oVertexNormal);
	vec3 lightDirection = normalize(lightPos - oPosInWorldSpace);
    float diff = max(dot(vertexNormal, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;
    diffuse *= attenuation;

    // Specular
	vec3 viewDir = normalize(viewPos - oPosInWorldSpace);
    vec3 halfwayDir = normalize(lightDirection + viewDir);
    float spec = pow(max(dot(vertexNormal, halfwayDir), 0.0), 32.0);

	vec3 specular = spec * lightColor;
    specular *= attenuation;

    vec3 phongLight = (ambient + diffuse + specular) * texture(textureWood, oTextureCoord).rgb;
    return phongLight;
}

void main()
{
    vec3 phongLight = AddLight();
    // if (oVertexNormal.y < 0 )
    // {
    //     FragColor = vec4(texture(textureWood, oTextureCoord).rgb, 0.5f);
    // }else
    // {
    //     FragColor = vec4(phongLight, 1.0);
    // }
    FragColor = vec4(phongLight, 1.0);
}