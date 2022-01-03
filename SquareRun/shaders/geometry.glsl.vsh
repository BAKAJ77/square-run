#version 330 core
layout(location = 0) in vec2 vertexCoords;
layout(location = 1) in vec2 uvCoords;

out VSH_OUT
{
    vec2 uvCoords;
} vshOut;

uniform mat4 mvpMatrix;

void main()
{
    gl_Position = mvpMatrix * vec4(vertexCoords, 0.0f, 1.0f);
    vshOut.uvCoords = uvCoords;
}
