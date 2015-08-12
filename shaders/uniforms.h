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
        static const char * _MIN_HEIGHT;
        static const char * _MAX_HEIGHT;
        static const char * _BASE_HEIGHT;
        static const char * _MATERIAL_AMBIENT;
        static const char * _MATERIAL_DIFFUSE;
        static const char * _SCALE;
    };

    struct Water{
        static const char * _MATERIAL_AMBIENT;
        static const char * _MATERIAL_DIFFUSE;
        static const char * _RENDER;
   };

    struct Texture{
        static const char * _TERRAIN_HEIGHTMAP;
        static const char * _WATER_HEIGHTMAP;
        static const char * _NORMALS;
        static const char * _SLOPE;
        static const char * _SHADE;
        static const char * _TEMPERATURE;
        static const char * _TEMPERATURE_JUN;
        static const char * _TEMPERATURE_DEC;
        static const char * _DAILY_ILLUMINATION;
        static const char * _MIN_DAILY_ILLUMINATION;
        static const char * _MAX_DAILY_ILLUMINATION;
        static const char * _SOIL_INFILTRATION_RATE;
        static const char * _CLUSTER_MEMBERSHIP;
        static const char * _MONTHLY_SOIL_HUMIDITY;
        static const char * _WEIGHTED_AVG_SOIL_HUMIDITY;
        static const char * _OVERLAY;
        static const char * _JAN_WEIGHTED_AVG_SOIL_HUMIDITY;
        static const char * _FEB_WEIGHTED_AVG_SOIL_HUMIDITY;
        static const char * _MAR_WEIGHTED_AVG_SOIL_HUMIDITY;
        static const char * _APR_WEIGHTED_AVG_SOIL_HUMIDITY;
        static const char * _MAY_WEIGHTED_AVG_SOIL_HUMIDITY;
        static const char * _JUN_WEIGHTED_AVG_SOIL_HUMIDITY;
        static const char * _JUL_WEIGHTED_AVG_SOIL_HUMIDITY;
        static const char * _AUG_WEIGHTED_AVG_SOIL_HUMIDITY;
        static const char * _SEP_WEIGHTED_AVG_SOIL_HUMIDITY;
        static const char * _OCT_WEIGHTED_AVG_SOIL_HUMIDITY;
        static const char * _NOV_WEIGHTED_AVG_SOIL_HUMIDITY;
        static const char * _DEC_WEIGHTED_AVG_SOIL_HUMIDITY;
        static const char * _WEIGHTED_AVG_SOIL_HUMIDITIES[12];
    };

    struct Overlay{
        static const char * _ACTIVE;
        static const char * _NONE;
        static const char * _SLOPE;
        static const char * _ALTITUDE;
        static const char * _SHADE;
        static const char * _TEMPERATURE;
        static const char * _MIN_DAILY_ILLUMINATION;
        static const char * _MAX_DAILY_ILLUMINATION;
        static const char * _SOIL_INFILTRATION_RATE;
        static const char * _MONTHLY_SOIL_HUMIDITY;
        static const char * _WEIGHTED_AVG_SOIL_HUMIDITY;
    };

    struct Clustering{
        static const char * _N_CLUSTERS;
        static const char * _JUN_TEMPERATURES;
        static const char * _DEC_TEMPERATURES;
        static const char * _MIN_ILLUMINATION;
        static const char * _MAX_ILLUMINATION;
        static const char * _SLOPE;
        static const char * _JAN_SOIL_HUMIDITY;
        static const char * _FEB_SOIL_HUMIDITY;
        static const char * _MAR_SOIL_HUMIDITY;
        static const char * _APR_SOIL_HUMIDITY;
        static const char * _MAY_SOIL_HUMIDITY;
        static const char * _JUN_SOIL_HUMIDITY;
        static const char * _JUL_SOIL_HUMIDITY;
        static const char * _AUG_SOIL_HUMIDITY;
        static const char * _SEP_SOIL_HUMIDITY;
        static const char * _OCT_SOIL_HUMIDITY;
        static const char * _NOV_SOIL_HUMIDITY;
        static const char * _DEC_SOIL_HUMIDITY;
        static const char * _SOIL_HUMIDITIES[12];
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

    struct WaterEdit {
        static const char * _ABSOLUTE_HEIGHT;
    };

    struct BorderOverlapReduction
    {
        static const char * _OVERLAP_MODE;
    };
};

#endif // UNIFORMS_H
