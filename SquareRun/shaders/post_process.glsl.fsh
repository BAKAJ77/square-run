#version 330 core

in VSH_OUT
{
    vec2 uvCoords;
} fshIn;

uniform sampler2DMS renderedTexture;
uniform vec2 framebufferSize;
uniform int numSamples;
uniform float gammaFactor;

vec3 GetMSAAFragmentColor()
{
    vec3 color;
    for (int i = 0; i < numSamples; i++)
        color += texelFetch(renderedTexture, ivec2(fshIn.uvCoords * framebufferSize), i).rgb;

    return color / float(numSamples);
}

vec3 GetGammaCorrectedColor(vec3 color)
{
    return pow(color, vec3(1.0f / gammaFactor));
}

void main()
{
    vec3 sampledFragColor = GetMSAAFragmentColor();
    gl_FragColor = vec4(GetGammaCorrectedColor(sampledFragColor), 1.0f);
}
