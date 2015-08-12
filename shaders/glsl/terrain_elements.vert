#version 430

layout(location = 0) in vec3 vPos;

// transformations
struct Transformation
{
  mat4 projMat;
  mat4 viewMat;
  mat4 mtwMat;
  float scale;
};

uniform Transformation transform;
uniform sampler2D terrain_height_map_texture;
uniform vec4 uniform_color;

out vec4 theColor;

void main()
{
    uvec2 terrain_size = textureSize(terrain_height_map_texture, 0);

    // Fetch the y coordinate from the heightmap texture
    vec4 world_space_pos = vec4(vPos.x * transform.scale, 0, vPos.z * transform.scale, 1);

    world_space_pos = transform.mtwMat * world_space_pos;

    vec2 tex_coord = vec2(world_space_pos.x/(terrain_size.x-1), world_space_pos.z/(terrain_size.y-1));

    // Load the y positions
    world_space_pos.y = (texture(terrain_height_map_texture, tex_coord).r+.8) * transform.scale;

    gl_Position = transform.projMat * transform.viewMat * world_space_pos;
    theColor = uniform_color;
}

