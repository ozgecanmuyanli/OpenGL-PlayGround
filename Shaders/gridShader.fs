#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;
in vec3 oPos;
in float finalHeight;

uniform sampler2D textureTerrain;
uniform vec3 viewPos;

void main()
{
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
	
	FragColor = vec4(vec3(finalColor * texture(textureTerrain, oTextureCoord).bbb), 1.0f); 
}