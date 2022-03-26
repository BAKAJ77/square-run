#version 330 core

in VSH_OUT
{
	vec2 uvCoords;
} fshIn;

uniform sampler2D fontBitmapTexture;
uniform vec4 textColor;

void main()
{
	gl_FragColor = vec4(textColor.rgb, textColor.a * texture(fontBitmapTexture, fshIn.uvCoords).r);
}
