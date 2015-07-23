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
    bool slope;
    bool altitude;
    bool shade;
    bool temperature;
    bool daily_illumination_min;
    bool daily_illumination_max;
    bool soil_infiltration_rate;
};

uniform Transformation transform;
uniform OverlayMode overlay;
uniform float max_height;
uniform float base_height;
uniform int month;

uniform sampler2D terrain_height_map_texture;
uniform usampler2D water_heightmap;
uniform sampler2D normals_texture;
uniform sampler2D shade_texture;
uniform sampler2D jun_temperature_texture;
uniform sampler2D dec_temperature_texture;
uniform sampler2D min_daily_illumination_texture;
uniform sampler2D max_daily_illumination_texture;
uniform usampler2D soil_infiltration_rate_texture;

out float slope;
out float altitude;
out float shade;
out float temperature;
out float min_daily_illumination;
out float max_daily_illumination;
out float soil_infiltration_rate;

out vec3 world_space_pos;

float mm_to_meters(in uint mm)
{
    return float(mm)/1000;
}

void main()
{
    // Fetch the y coordinate from the heightmap texture
    world_space_pos = vec3(vPos.x*transform.scale, texture(terrain_height_map_texture, textureCoord).r * transform.scale, vPos.z * transform.scale);
    uint water_height = texture(water_heightmap, textureCoord).r;
    if(water_height > 0)
    {
        world_space_pos.y += mm_to_meters(water_height);
    }
    world_space_pos.y += 1; // Add one for the overlays

    gl_Position = transform.projMat * transform.viewMat * vec4(world_space_pos, 1.0);

    if(overlay.slope)
    {
        vec3 normal = texture(normals_texture, textureCoord).rgb;
        vec3 vertical_vector = vec3(0,1,0);
        slope = 1 - abs(dot(normal, vertical_vector));
    }
    else if(overlay.altitude)
    {
        altitude = ((world_space_pos.y-1)-base_height)/(max_height-base_height);
    }
    else if(overlay.temperature)
    {
        float jun_temperature = texture(jun_temperature_texture, textureCoord).r * 127;
        float dec_temperature = texture(dec_temperature_texture, textureCoord).r * 127;
        float temp_diff = jun_temperature - dec_temperature;

        float jun_percentage = (6.0-(abs(6-month)))/6.0;

        temperature = dec_temperature + (jun_percentage * temp_diff);
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
    else if(overlay.soil_infiltration_rate)
    {
        soil_infiltration_rate = float(texture(soil_infiltration_rate_texture, textureCoord).r);
    }
}

