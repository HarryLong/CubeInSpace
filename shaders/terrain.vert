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
};

uniform Transformation transform;
uniform OverlayMode overlay;
uniform float max_height;
uniform float base_height;

uniform sampler2D height_map_texture;
uniform sampler2D normals_texture;

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
out vec3 pos;

void main()
{
    // Fetch the y coordinate from the texture
    vec3 v = vec3(vPos.x, texture(height_map_texture, textureCoord).r, vPos.z);
    pos = v;

    // Calculate the slope
    vec3 normal = texture(normals_texture, textureCoord).rgb;
    vec3 x_y_plane = vec3(1,0,0);
    slope = abs(dot(normal, x_y_plane));

    // Calculate the altitude
    altitude = ((v.y-base_height)/max_height-base_height);

    // map normal to camera space for lighting calculations
    camera_space_normal = normalize(mat3(transform.viewMat) * normal);

    vec4 camera_space_pos = transform.viewMat * vec4(v, 1.0);
    vec4 camera_space_light_pos = transform.viewMat * light_position;

    light_direction = normalize(camera_space_light_pos.xyz - camera_space_pos.xyz);

    half_vector = normalize(normalize(-camera_space_pos.xyz) + light_direction);

    diffuse = material_diffuse * light_diffuse_color;
    ambient = material_ambient * light_ambient_color;

    gl_Position = transform.projMat * camera_space_pos;
}

