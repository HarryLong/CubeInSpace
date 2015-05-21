#version 330

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 textureCoord;

// transformations
struct Transformation
{
  mat4 projMat;
  mat4 viewMat;
  float scale;
};

uniform Transformation transform;

uniform sampler2D height_map_texture;
out vec4 theColor;

void main()
{
    // Fetch the y coordinate from the heightmap texture
    vec3 world_space_pos = vec3(vPos.x, texture(height_map_texture, textureCoord).r + 1, vPos.z);
//    vec3 world_space_pos = vec3(vPos.x, 5, vPos.z);

    gl_Position = transform.projMat * transform.viewMat * vec4(world_space_pos, 1.0);;

    theColor = vec4(0.3, 0.3, 0.3, 0.5);
}

