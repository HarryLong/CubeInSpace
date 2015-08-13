#version 430

layout(location = 0) in vec3 vPos;
//layout(location = 1) in vec2 textureCoord;

// transformations
struct Transformation
{
  mat4 projMat;
  mat4 viewMat;
  float scale;
};

uniform Transformation transform;

uniform sampler2D terrain_height_map_texture;
uniform sampler2D water_heightmap;
uniform sampler2D overlay_texture;
uniform sampler2D normals_texture;
uniform bool render_water;

//colours and material
uniform vec4 terrain_material_diffuse;
uniform vec4 terrain_material_ambient;
uniform vec4 water_material_diffuse;
uniform vec4 water_material_ambient;
uniform vec3 light_position; // in world space
uniform vec4 light_diffuse_color;
uniform vec4 light_ambient_color;

uniform float base_height;
uniform bool overlay_active;
uniform float terrain_scale;

out vec3 camera_space_normal; // vertex normal
out vec3 light_direction; // toLight
out vec3 half_vector;
out vec4 diffuse;
out vec4 ambient;
out vec4 overlay_color;

out vec3 world_space_pos;

//float to_terrain_scale(in uint mm)
//{
//    return float(mm)/1000.0/terrain_scale;
//}

void main()
{
    ivec2 texture_size = textureSize(terrain_height_map_texture,0);
    vec2 texture_coord = vec2(float(vPos.x)/(texture_size.x-1), float(vPos.z)/(texture_size.y-1));

    // Fetch the y coordinate from the heightmap texture
    world_space_pos = vec3(vPos.x, texture(terrain_height_map_texture, texture_coord).r, vPos.z);
    float water_height = texture(water_heightmap, texture_coord).r;
    if(!render_water || water_height*terrain_scale*1000 < 10) // Don't render if under 1 cm
    {
        water_height = 0;
    }
    else
    {
        world_space_pos.y += water_height;
    }

    vec4 camera_space_pos = transform.viewMat * vec4(world_space_pos, 1.0);
    gl_Position = transform.projMat * camera_space_pos;

    vec3 normal = texture(normals_texture, texture_coord).rgb;
    // map normal to camera space for lighting calculations
    camera_space_normal = normalize(mat3(transform.viewMat) * normal);

    if(water_height > 0) // water
    {
        diffuse = water_material_diffuse * light_diffuse_color;
        ambient = water_material_ambient * light_ambient_color;
    }
    else // bear terrain
    {
        diffuse = terrain_material_diffuse * light_diffuse_color;
        ambient = terrain_material_ambient * light_ambient_color;
    }

    if(light_position.y >= base_height) // Day
    {
        vec4 camera_space_light_pos = transform.viewMat * vec4(light_position,1);
        light_direction = normalize(camera_space_light_pos.xyz - camera_space_pos.xyz);
        half_vector = normalize(normalize(-camera_space_pos.xyz) + light_direction);
    }
    else // Night
    {
        light_direction = vec3(0,0,0);
        half_vector = vec3(0,0,0);
    }

    if(overlay_active)
    {
        overlay_color = texture(overlay_texture, texture_coord).rgba;
    }
}

