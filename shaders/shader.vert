#version 330

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec4 vertexColor;

uniform mat4 projectionMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 viewMatrix;

out vec4 theColor;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelToWorldMatrix * vertexPosition;

    theColor = vertexColor;
}
