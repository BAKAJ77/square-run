#version 330 core

uniform sampler2DMS postProcessedTexture;
uniform vec2 framebufferSize;
uniform int numSamples;
uniform float gammaFactor;

vec4 GetSampledFragmentColor()
{
    vec4 color;
    for (int i = 0; i < numSamples; i++)
        color += texelFetch(postProcessedTexture, ivec2(gl_FragCoord), i);

    return color / float(numSamples);
}

vec4 GetGammaCorrectedColor(vec4 color)
{
    return vec4(pow(color.rgb, vec3(1.0f / gammaFactor)), color.a);
}

void main()
{
    vec4 sampledColor = GetSampledFragmentColor();
	gl_FragColor = GetGammaCorrectedColor(sampledColor);
}
