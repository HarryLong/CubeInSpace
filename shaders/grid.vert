#version 330

layout(location = 0) in vec3 vPos;

struct Transformation
{
  mat4 projMat;
  mat4 viewMat;
};

uniform Transformation transform;

uniform  bool drawing_grid;

out vec4 theColor;

void main()
{
    gl_Position = transform.projMat * transform.viewMat * vec4(vPos,1);
    if(drawing_grid)
    {
        if(vPos.x == 0 || vPos.z == 0)
            theColor = vec4(.8f, .8f, .8f, 1.f);
        else
            theColor = vec4(.2f, .2f, .2f, 1.f);
    }
    else
    {
        theColor = vec4(.294f, .0f, .51f, 1.f);
    }
}

