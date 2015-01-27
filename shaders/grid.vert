#version 330

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec4 color;

struct Transformation
{
  mat4 projMat;
  mat4 viewMat;
};

uniform Transformation transform;

out vec4 theColor;

void main()
{
    gl_Position = transform.projMat * transform.viewMat * vec4(vPos,1);;
    theColor = color;
}

