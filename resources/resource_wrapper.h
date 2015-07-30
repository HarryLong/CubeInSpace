#ifndef RESOURCE_WRAPPER_H
#define RESOURCE_WRAPPER_H

#include "terrain_shade.h"
#include "terrain_daily_illumination.h"
#include "terrain_temperature.h"
#include "terrain_water.h"
#include "soil_infiltration.h"
#include "soil_humidity.h"
#include <atomic>

class LightingManager;
class ResourceWrapper : public QObject{
Q_OBJECT
public:
    ResourceWrapper();
    ~ResourceWrapper();

    void syncTextures();

    void getResourceInfo(const glm::vec2 & pos, int month, float & water_height, bool & shaded, int & min_illumination, int & max_illumination, float & temp,
                         int & soil_infiltration_rate, int & soil_humidity);

    void valid(bool & shade, bool & daily_illumination, bool & temp);
    void refreshShade(Terrain & terrain, const glm::vec3 & sun_position);
    void refreshDailyIllumination(LightingManager & lighting_manager, Terrain & terrain);
    void refreshTemperature(const Terrain & terrain, float temp_at_zero_june, float lapse_rate_june, float temp_at_zero_dec, float lapse_rate_dec);
//    void refreshWater(int terrain_width, int terrain_depth, int rainfall_jun, int rainfal_intensity_jun, int rainfall_dec, int rainfal_intensity_dec);

    TerrainWater & getTerrainWater();
    SoilInfiltration & getSoilInfiltration();
    SoilHumidity & getSoilHumidity();

    bool recalculatingShade();
    bool recalculatingDailyIllumination();
    bool recalculatingTemperature();
    bool recalculatingWater();

signals:
    void processing(QString description);
    void processingUpdate(int percent_complete);
    void processingComplete();
    void processDescriptionUpdate(QString description);
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
//    void bindJunWater();
//    void bindDecWater();

//    bool isShaded(const glm::vec3 & point, bool & shaded);
//    bool getTemperature(const glm::vec3 & point, int month, float & temperature);
//    bool getDailyMinMaxIllumination(const glm::vec3 & point, int & min, int & max);
//    bool getWaterHeight(const glm::vec3 & point, int month, int & height);

private:
    GLubyte * get_shade(Terrain & terrain, const glm::vec3 & sun_position, bool emit_progress_updates = true);

    TerrainShade m_terrain_shade;
    TerrainDailyIllumination m_terrain_daily_illumination;
    TerrainTemperature m_terrain_temp;
    TerrainWater m_terrain_water;
    SoilInfiltration m_soil_infiltration;
    SoilHumidity m_soil_humidity;

    std::atomic<bool> m_recalculating_shade;
    std::atomic<bool> m_recalculating_daily_illumination;
    std::atomic<bool> m_recalculating_temperature;
    std::atomic<bool> m_recalculating_water;
};

#endif // RESOURCE_WRAPPER_H
