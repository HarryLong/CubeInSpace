#version 330

layout (location=0) in vec2 vertex;

void main(void)
{
    gl_Position = vec4(vertex.x, vertex.y, 0.0, 1.0); // already in clip space for ortho cam
}
