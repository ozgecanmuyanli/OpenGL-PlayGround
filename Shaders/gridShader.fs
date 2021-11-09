#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;
in vec3 oPos;
in float finalHeight;

uniform sampler2D textureNoise;
uniform vec3 viewPos;

void main()
{
    vec3 finalColor;
	if(texture(textureNoise, oTextureCoord).r < 0.5f) 
	{
		//finalColor = vec3(texture(textureNoise, oTextureCoord).r, texture(textureNoise, oTextureCoord).r, texture(textureNoise, oTextureCoord).r + 0.4);
        discard;
    }
    else
    {
        finalColor = texture(textureNoise, oTextureCoord).rrr;
    }

	FragColor = vec4(finalColor, 1.0f); 
}