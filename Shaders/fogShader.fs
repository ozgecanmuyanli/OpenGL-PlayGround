#version 330 core
out vec4 FragColor;

in vec2 oTextureCoord;
in vec3 oVertexNormal;
in vec4 oEyeSpacePos;

uniform sampler2D wallTexture;

struct FogParameters
{
    vec3 color;
    float linearStart;
    float linearEnd;
    float density;
    int equation;
};

float getFogFactor(FogParameters params, float fogCoordinate)
{
    float result = 0.0;
    if(params.equation == 0)
    {
        float fogLength = params.linearEnd - params.linearStart;
        result = (params.linearEnd - fogCoordinate) / fogLength;
    }
    else if(params.equation == 1)
    {
        result = exp(-params.density * fogCoordinate);
    }
    else if(params.equation == 2)
    {
        result = exp(-pow(params.density * fogCoordinate, 2.0));
    }
    result = 1.0 - clamp(result, 0.0, 1.0);
    return result;
}

uniform FogParameters fogParams;

void main()
{
    vec4 outputColor;

    float fogCoordinate = abs(oEyeSpacePos.z / oEyeSpacePos.w);
    outputColor = mix(texture(wallTexture, oTextureCoord), vec4(fogParams.color, 1.0), getFogFactor(fogParams, fogCoordinate));

    FragColor = outputColor;
}