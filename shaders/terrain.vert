#version 430

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 textureCoord;

// transformations
struct Transformation
{
  mat4 projMat;
  mat4 viewMat;
  float scale;
};

struct OverlayMode
{
    bool none;
    bool slope;
    bool altitude;
    bool shade;
    bool temperature_min;
    bool temperature_max;
    bool daily_illumination_min;
    bool daily_illumination_max;
};

uniform Transformation transform;
uniform OverlayMode overlay;
uniform float max_height;
uniform float base_height;

uniform sampler2D terrain_height_map_texture;
uniform usampler2D water_height_map_texture;
uniform sampler2D normals_texture;
uniform sampler2D shade_texture;
uniform sampler2D min_temp_texture;
uniform sampler2D max_temp_texture;
uniform sampler2D min_daily_illumination_texture;
uniform sampler2D max_daily_illumination_texture;

//colours and material
uniform vec4 terrain_material_diffuse;
uniform vec4 terrain_material_ambient;
uniform vec4 water_material_diffuse;
uniform vec4 water_material_ambient;
uniform vec4 light_position; // in world space
uniform vec4 light_diffuse_color;
uniform vec4 light_ambient_color;

out vec3 camera_space_normal; // vertex normal
out vec3 light_direction; // toLight
out vec3 half_vector;
out vec4 diffuse;
out vec4 ambient;

out float slope;
out float altitude;
out float shade;

out float min_temperature;
out float max_temperature;

out float min_daily_illumination;
out float max_daily_illumination;

float mm_to_meters(in uint mm)
{
    return float(mm)/1000;
}

void main()
{
    // Fetch the y coordinate from the heightmap texture
    vec3 world_space_pos = vec3(vPos.x*transform.scale, texture(terrain_height_map_texture, textureCoord).r * transform.scale, vPos.z * transform.scale);

    uint water_height = texture(water_height_map_texture, textureCoord).r;
    if(water_height > 0)
    {
        world_space_pos.y += mm_to_meters(water_height);
    }

    vec4 camera_space_pos = transform.viewMat * vec4(world_space_pos, 1.0);
    gl_Position = transform.projMat * camera_space_pos;

    if(overlay.altitude)
    {
        altitude = (world_space_pos.y-base_height)/(max_height-base_height);
    }
    else if(overlay.temperature_min)
    {
        min_temperature = texture(min_temp_texture, textureCoord).r * 127;
    }
    else if(overlay.temperature_max)
    {
        max_temperature = texture(max_temp_texture, textureCoord).r * 127;
    }
    else if(overlay.shade)
    {
        if(texture(shade_texture, textureCoord).r > 0)
        {
            shade = 1;
        }
        else
        {
            shade = 0;
        }
    }
    else if(overlay.daily_illumination_min)
    {
        min_daily_illumination = texture(min_daily_illumination_texture, textureCoord).r * 255;
    }
    else if(overlay.daily_illumination_max)
    {
        max_daily_illumination = texture(max_daily_illumination_texture, textureCoord).r * 255;
    }
    else // Default
    {
        vec3 normal = texture(normals_texture, textureCoord).rgb;
        if(overlay.slope)
        {
            vec3 vertical_vector = vec3(0,1,0);
            slope = 1 - abs(dot(normal, vertical_vector));
        }
        else // Default
        {
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
                vec4 camera_space_light_pos = transform.viewMat * light_position;
                light_direction = normalize(camera_space_light_pos.xyz - camera_space_pos.xyz);
                half_vector = normalize(normalize(-camera_space_pos.xyz) + light_direction);
            }
            else // Night
            {
                light_direction = vec3(0,0,0);
                half_vector = vec3(0,0,0);
            }
        }
    }
}

