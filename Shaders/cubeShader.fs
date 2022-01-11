#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;

uniform sampler2D entityTexture;

float near = 0.1f;
float far = 2.0f;

float LinearizeDepth(float depth)
{
	float ndc = depth * 2.0f - 1.0f; // back to NDC
	float linearDepth = (2.0f * near * far) / (far + near - ndc * (far - near));
	return linearDepth;
}

void main()
{
	//FragColor = texture(entityTexture, oTextureCoord);
	float depth = LinearizeDepth(gl_FragCoord.z) / far; // / far for demo
	FragColor = vec4(vec3(depth), 1.0);
}