#ifndef RESOURCE_WRAPPER_H
#define RESOURCE_WRAPPER_H

#include "terrain_shade.h"
#include "terrain_daily_illumination.h"
#include "terrain_temperature.h"
#include "terrain_water.h"

class LightingManager;
class ResourceWrapper : public QObject{
Q_OBJECT
public:
    ResourceWrapper();
    ~ResourceWrapper();

    void syncTextures();

    void getResourceInfo(const glm::vec2 & pos, int month, int & water_height, bool & shaded, int & min_illumination, int & max_illumination, float & temp);

    void valid(bool & shade, bool & daily_illumination, bool & temp);
    void refreshShade(Terrain & terrain, const glm::vec3 & sun_position);
    void refreshDailyIllumination(LightingManager & lighting_manager, Terrain & terrain);
    void refreshTemperature(const Terrain & terrain, float temp_at_zero_june, float lapse_rate_june, float temp_at_zero_dec, float lapse_rate_dec);
    void refreshWater(int terrain_width, int terrain_depth, int rainfall_jun, int rainfal_intensity_jun, int rainfall_dec, int rainfal_intensity_dec);

signals:
    void processing(QString description);
    void processingUpdate(int percent_complete);
    void processingComplete();
    void resourceInvalidated();

public slots:
    void terrainChanged();
    void latitudeChanged();
    void sunPositionChanged();
    void bindShade();
    void bindMinIllumination();
    void bindMaxIllumination();
    void bindJunTemperature();
    void bindDecTemperature();
    void bindJunWater();
    void bindDecWater();

//    bool isShaded(const glm::vec3 & point, bool & shaded);
//    bool getTemperature(const glm::vec3 & point, int month, float & temperature);
//    bool getDailyMinMaxIllumination(const glm::vec3 & point, int & min, int & max);
//    bool getWaterHeight(const glm::vec3 & point, int month, int & height);

private:
    TerrainShade m_terrain_shade;
    TerrainDailyIllumination m_terrain_daily_illumination;
    TerrainTemperature m_terrain_temp;
    TerrainWater m_terrain_water;
};

#endif // RESOURCE_WRAPPER_H
