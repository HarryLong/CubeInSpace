#version 330

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec4 color;

// transformations
struct Transformation
{
  mat4 projMat;
  mat4 viewMat;
  mat4 modelMat;
  float scale;
};

uniform Transformation transform;

out vec4 theColor;

void main()
{
    gl_Position = transform.projMat * transform.viewMat * transform.modelMat * vec4(vPos.x * transform.scale, vPos.y * transform.scale, vPos.z * transform.scale,1);;
//    theColor = vec4(vColor,1);
    theColor = color;
//    theColor = vec4(1,0,0,1);
}

