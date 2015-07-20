#version 430

const vec4 color_at_minus_fifty_degrees = vec4(0,0,1,.7);
const vec4 color_at_plus_fifty_degrees = vec4(1,0,0,.7);

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

uniform OverlayMode overlay;

in float slope;
in float altitude; // ratio over max
in float shade;
in float temperature;
in float min_daily_illumination;
in float max_daily_illumination;
in float soil_infiltration_rate;

out vec4 outputColor;

vec4 temp_to_output_color(in float temperature)
{
    float color_at_plus_fifty_degrees_percentage = (temperature+50)/100.0;
    float color_at_minus_fifty_degrees_percentage = 1-color_at_plus_fifty_degrees_percentage;

    return (color_at_plus_fifty_degrees_percentage * color_at_plus_fifty_degrees) +
                (color_at_minus_fifty_degrees_percentage * color_at_minus_fifty_degrees);
}

vec4 slope_to_output_color(in float slope)
{
    return vec4(slope,slope,slope,.7);
}

vec4 altitude_to_output_color(in float altitude)
{
    return vec4(altitude,altitude,altitude,.7);
}

vec4 shade_to_output_color(in float shade)
{
    if(shade > 0) // Shaded
    {
        return vec4(0.01, 0.01, 0.01, .7);
    }
    else // Not shaded
    {
        return vec4(0.7,0.7,0.7,.7);
    }
}

vec4 daily_illumination_to_output_color(in float daily_illumination)
{
    float intensity = daily_illumination/24;
    return vec4(intensity,intensity,intensity,.7);
}

vec4 soil_infiltration_rate_to_output_color(in float soil_infiltration_rate)
{
    float intensity = soil_infiltration_rate/500.0; // 500 is the max
    return vec4(0,0,intensity,.7);
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
    else if(overlay.temperature)
    {
        outputColor = temp_to_output_color(temperature);
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
    else if(overlay.soil_infiltration_rate)
    {
        outputColor = soil_infiltration_rate_to_output_color(soil_infiltration_rate);
    }
}
