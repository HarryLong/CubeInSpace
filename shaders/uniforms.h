#ifndef UNIFORMS_H
#define UNIFORMS_H

class Uniforms{
public:
    struct Transform{
        static const char * _PROJECTION;
        static const char *_VIEW;
        static const char * _MODEL;
        static const char * _SCALE;
    };

    struct Terrain{
        static const char * _MAX_HEIGHT;
        static const char * _BASE_HEIGHT;
        static const char * _MATERIAL_AMBIENT;
        static const char * _MATERIAL_DIFFUSE;
    };

    struct Water{
        static const char * _MATERIAL_AMBIENT;
        static const char * _MATERIAL_DIFFUSE;
   };

    struct Texture{
        static const char * _TERRAIN_HEIGHTMAP;
        static const char * _WATER_HEIGHTMAP;
        static const char * _NORMALS;
        static const char * _SHADE;
        static const char * _TEMPERATURE_JUN;
        static const char * _TEMPERATURE_DEC;
        static const char * _MIN_DAILY_ILLUMINATION;
        static const char * _MAX_DAILY_ILLUMINATION;
        static const char * _SOIL_INFILTRATION_RATE;
    };

    struct Overlay{
        static const char * _NONE;
        static const char * _SLOPE;
        static const char * _ALTITUDE;
        static const char * _SHADE;
        static const char * _TEMPERATURE;
        static const char * _MIN_DAILY_ILLUMINATION;
        static const char * _MAX_DAILY_ILLUMINATION;
        static const char * _SOIL_INFILTRATION_RATE;
    };

    struct Lighting{
        static const char * _POSITION;
        static const char * _DIFFUSE_COLOR;
        static const char * _AMBIENT_COLOR;
    };

    struct Assets {
        static const char * _UNIFORM_COLOR;
        static const char * _USE_UNIFORM_COLOR;
    };

    struct Timing {
        static const char * _MONTH;
    };

    struct Counter {
        static const char * _VERTICIES_EMPTIED;
    };

    struct Rainfall{
        static const char * _RAINFALL;
        static const char * _INTENSITY;
    };

    struct SlopeBasedSoilInfiltration{
        static const char * _MIN_SLOPE;
    };
};

#endif // UNIFORMS_H
