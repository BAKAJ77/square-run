#version 330 core

in VSH_OUT
{
	vec2 uvCoords;
} fshIn;

uniform sampler2DMS postProcessedTexture;
uniform vec2 framebufferSize;
uniform int numSamples;

vec4 GetFinalFragmentColor()
{
    vec4 color;
    for (int i = 0; i < numSamples; i++)
        color += texelFetch(postProcessedTexture, ivec2(fshIn.uvCoords * framebufferSize), i);

    return color / float(numSamples);
}

void main()
{
	gl_FragColor = GetFinalFragmentColor();
}
