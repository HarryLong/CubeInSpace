#version 330

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 textureCoord;

// transformations
struct Transformation
{
  mat4 projMat;
  mat4 viewMat;
  mat4 mtwMat;
  float scale;
};

uniform Transformation transform;
uniform sampler2D height_map_texture;
uniform vec4 uniform_color;
uniform float height_scale;

out vec4 theColor;

void main()
{
    // Fetch the y coordinate from the heightmap texture
    vec3 world_space_pos = vec3(vPos.x * transform.scale, (texture2D(height_map_texture, textureCoord).r * transform.scale) + 1, vPos.z * transform.scale);
//    vec3 world_space_pos = vec3(vPos.x, 5, vPos.z);

    gl_Position = transform.projMat * transform.viewMat * transform.mtwMat * vec4(world_space_pos, 1.0);;

    theColor = uniform_color;
}

