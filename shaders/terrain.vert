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

struct OverlayMode
{
    bool none;
    bool slope;
    bool altitude;
    bool shade;
};

uniform Transformation transform;
uniform OverlayMode overlay;
uniform float max_height;
uniform float base_height;
uniform float height_scale;

uniform sampler2D height_map_texture;
uniform sampler2D normals_texture;
uniform sampler2D shade_texture;

//colours and material
uniform vec4 material_diffuse;
uniform vec4 material_ambient;
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

void main()
{
    // Fetch the y coordinate from the heightmap texture
    vec3 world_space_pos = vec3(vPos.x, height_scale * texture(height_map_texture, textureCoord).r, vPos.z);
    vec4 camera_space_pos = transform.viewMat * vec4(world_space_pos, 1.0);
    gl_Position = transform.projMat * camera_space_pos;

    if(overlay.none || overlay.slope) // Normals needs to be loaded
    {
        vec3 normal = texture(normals_texture, textureCoord).rgb;
        if(overlay.none)
        {
            // map normal to camera space for lighting calculations
            camera_space_normal = normalize(mat3(transform.viewMat) * normal);

            diffuse = material_diffuse * light_diffuse_color;
            ambient = material_ambient * light_ambient_color;
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
        else // slope
        {
            vec3 x_y_plane = vec3(0,1,0);
            slope = 1 - abs(dot(normal, x_y_plane));
        }
    }
    else if(overlay.altitude)
    {
        altitude = ((world_space_pos.y-base_height)/max_height-base_height);
    }
    else if(overlay.shade)
    {
//        shade = texture(shade_texture, textureCoord).r;
        if(texture(shade_texture, textureCoord).r > 0)
        {
            shade = 1;
        }
        else
        {
            shade = 0;
        }
    }
}

