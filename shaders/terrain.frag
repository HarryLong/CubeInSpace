#version 330

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
in float shade;
in float min_temperature;
in float max_temperature;
in float min_daily_illumination;
in float max_daily_illumination;

out vec4 outputColor;

vec4 temp_to_output_color(in float temp)
{
    if(temp <= -10) // Blue color band
    {
        return vec4(0,0,temp/-50.0, 1.0);
    }
    else if(temp <= 10) // White color band
    {
        float strength = (temp+10.0)/20.0;
        return vec4(strength,strength,strength, 1.0);
    }
    else // Red color band
    {
        return vec4(temp/50.0,0,0, 1.0);
    }
}

vec4 slope_to_output_color(in float slope)
{
    return vec4(slope,slope,slope,1.0);
}

vec4 altitude_to_output_color(in float altitude)
{
    return vec4(altitude,altitude,altitude,1.0);
}

vec4 shade_to_output_color(in float shade)
{
    if(shade > 0) // Shaded
    {
        return vec4(0.01, 0.01, 0.01, 1.0);
    }
    else // Not shaded
    {
        return vec4(0.8,0.8,0.8,1.0);
    }
}

vec4 daily_illumination_to_output_color(in float daily_illumination)
{
    float intensity = daily_illumination/24;
    return vec4(intensity,intensity,intensity,1.0);
}

void main()
{
    if(overlay.altitude)
    {
        outputColor = altitude_to_output_color(altitude);
    }
    else if(overlay.slope)
    {
        outputColor = slope_to_output_color(slope);
    }
    else if(overlay.temperature_min)
    {
        outputColor = temp_to_output_color(min_temperature);
    }
    else if(overlay.temperature_max)
    {
        outputColor = temp_to_output_color(max_temperature);
    }
    else if(overlay.shade)
    {
        outputColor = shade_to_output_color(shade);
    }
    else if(overlay.daily_illumination_min)
    {
        outputColor = daily_illumination_to_output_color(min_daily_illumination);
    }
    else if(overlay.daily_illumination_max)
    {
        outputColor = daily_illumination_to_output_color(max_daily_illumination);
    }
    else // Default
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
        }
    }
}
