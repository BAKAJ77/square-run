#version 330 core
layout(location = 0) in vec2 vertexCoords;

void main()
{
    gl_Position = vec4(vertexCoords, 0.0f, 1.0f);
}
