#version 330

struct OverlayMode
{
    bool none;
    bool slope;
    bool altitude;
};

uniform vec4 specularCol;
uniform float shiny;
uniform vec4 matSpec;
uniform OverlayMode overlay;

in vec3 camera_space_normal; // vertex normal
in vec3 light_direction; // toLight
in vec3 half_vector;
in vec4 diffuse;
in vec4 ambient;
in float slope;
in float altitude; // ratio over max
in vec3 pos;

out vec4 outputColor;

void main()
{
    if(overlay.none)
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

//            if(camera_space_normal.xyz == 0.0)
//            {
//                outputColor = vec4(1.0,0.0,0.0,1.0);
//            }
//            else if(half_vector.xyz == 0.0)
//            {
//                outputColor = vec4(0.0,1.0,0.0,1.0);
//            }
//            else
//            {
//                outputColor += matSpec * specularCol * pow(normal_dot_half_vector, shiny);
//            }
//        }
//        else
//        {
//            outputColor = vec4(0.0,0.0,1.0,1.0);
//        }
        }
    }
    else if(overlay.altitude)
    {
        outputColor = vec4(altitude,altitude,altitude,1.0);
    }
    else if(overlay.slope)
    {
        outputColor = vec4(slope,slope,slope,1.0);
    }
}
