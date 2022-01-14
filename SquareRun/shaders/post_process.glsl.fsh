#version 330 core

in VSH_OUT
{
    vec2 uvCoords;
} fshIn;

uniform sampler2DMS processedTexture;
uniform vec2 framebufferSize;
uniform int numSamples;

vec3 GetSampledFragmentColor()
{
    vec3 color;
    for (int i = 0; i < numSamples; i++)
        color += texelFetch(processedTexture, ivec2(fshIn.uvCoords * framebufferSize), i).rgb;

    return color / float(numSamples);
}

void main()
{
    gl_FragColor = vec4(GetSampledFragmentColor(), 1.0f);
}
