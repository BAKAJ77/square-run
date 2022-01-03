#version 330 core

in VSH_OUT
{
	vec2 uvCoords;
} fshIn;

uniform sampler2D geometryTexture;
uniform vec4 geometryColor;
uniform bool enableTextureUsage;

void main()
{
	if (enableTextureUsage)
		gl_FragColor = texture(geometryTexture, fshIn.uvCoords);
	else
		gl_FragColor = geometryColor;
}
