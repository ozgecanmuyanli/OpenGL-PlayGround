#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;
in vec3 oVertexNormal;
in vec3 oPosInWorldSpace;

uniform sampler2D entityTexture;
uniform vec3 lightDir;
uniform vec3 viewPos;

float near = 0.1f;
float far = 2.0f;
float LinearizeDepth(float depth)
{
	float ndc = depth * 2.0f - 1.0f; // back to NDC
	float linearDepth = (2.0f * near * far) / (far + near - ndc * (far - near));
	return linearDepth;
}

vec3 AddLight()
{
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);//
    float ambientStrength = 0.5;
    float specularStrength = 0.5f;

	// Amnient
    vec3 ambient = ambientStrength * lightColor;

	// Diffuse
	vec3 vertexNormal = normalize(oVertexNormal);
	vec3 lightDirection = normalize(-lightDir);
	//vec3 lightDirection = normalize(lightPos - oPosInWorldSpace);
    float diff = max(dot(vertexNormal, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
	vec3 viewDir = normalize(viewPos - oPosInWorldSpace);
    vec3 reflectDir = reflect(-lightDir, vertexNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = spec * lightColor;

    vec3 phongLight = (ambient + diffuse + specular);
    return phongLight;
}


void main()
{
	FragColor = texture(entityTexture, oTextureCoord) * vec4(AddLight(), 1.0f);
	float depth = LinearizeDepth(gl_FragCoord.z) / far;
	//FragColor = vec4(vec3(depth), 1.0);
}