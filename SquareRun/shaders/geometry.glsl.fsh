#version 330 core

struct Material
{
	sampler2D texture;
	vec4 color;
	bool useTexture;
};

in VSH_OUT
{
	vec2 uvCoords;
} fshIn;

uniform Material material;

void main()
{
	if (material.useTexture)
		gl_FragColor = texture(material.texture, fshIn.uvCoords);
	else
		gl_FragColor = material.color;
}
