#version 330 core
layout(location = 0) in vec2 vertexCoords;

uniform mat4 MVPMatrix;

void main()
{
    gl_Position = MVPMatrix * vec4(vertexCoords, 0.0f, 1.0f);
}
