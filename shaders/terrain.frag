#version 430

const float contour_width = 2;
const float contour_separation = 10;

uniform vec4 specularCol;
uniform float shiny;
uniform vec4 matSpec;

in vec3 camera_space_normal; // vertex normal
in vec3 light_direction; // toLight
in vec3 half_vector;
in vec4 diffuse;
in vec4 ambient;

in vec3 world_space_pos;

out vec4 outputColor;

void main()
{
    outputColor = ambient;
    float normal_dot_lightdir = dot(camera_space_normal.xyz, light_direction);
    if(normal_dot_lightdir > 0.0) // i.e light direction TO normal in range [-89,89]
    {
        outputColor += diffuse * normal_dot_lightdir;
        float normal_dot_half_vector = dot(camera_space_normal, half_vector);

        if(normal_dot_half_vector > 0.0)
        {
            outputColor += matSpec * specularCol * pow(normal_dot_half_vector, shiny);
        }

        // Contour lines
        float f  = abs(fract (world_space_pos.y * 0.5) - 0.5);
        float df = fwidth(world_space_pos.y * 0.5);
        float g = smoothstep(-1.0*df, 1.0*df , f);

        float c = g;
        outputColor = vec4(c,c,c,1.0) * outputColor;
    }
}
