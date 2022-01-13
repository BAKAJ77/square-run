#version 330 core

in VSH_OUT
{
	vec2 uvCoords;
} fshIn;

uniform sampler2D geometryTexture;

void main()
{
	gl_FragColor = texture(geometryTexture, fshIn.uvCoords);
}
