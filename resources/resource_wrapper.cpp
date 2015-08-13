#include "resource_wrapper.h"
#include "../lighting/lighting_manager.h"
#include "../terrain/terrain.h"
#include <cstring>

ResourceWrapper::ResourceWrapper() :
    m_temp_valid(false),
    m_daily_illumination_valid(false),
    m_shade_valid(false)
{
}

ResourceWrapper::~ResourceWrapper()
{

}

void ResourceWrapper::valid(bool & shade, bool & daily_illumination, bool & temp)
{
    shade = m_shade_valid;
    daily_illumination = m_daily_illumination_valid;
    temp = m_temp_valid;
}

void ResourceWrapper::terrainChanged(int width, int depth)
{
    m_terrain_shade.setDimensions(width, depth);

    m_terrain_daily_illumination.getMin().setDimensions(width, depth);
    m_terrain_daily_illumination.getMax().setDimensions(width, depth);

    m_terrain_temp.setDimensions(width, depth);

    m_terrain_water.setDimensions(width, depth);

    m_soil_humidity.setDimensions(width, depth);

    m_weighted_soil_humidity.setDimensions(width, depth);

    m_soil_infiltration.reset(width, depth);

    m_slope.setDimensions(width, depth);

    m_terrain_shade.setDimensions(width, depth);

    m_temp_valid = false;
    m_daily_illumination_valid = false;
    m_shade_valid = false;

    emit resourceInvalidated();
}

void ResourceWrapper::latitudeChanged()
{
    m_shade_valid = false;
    m_daily_illumination_valid = false;

    emit resourceInvalidated();
}

void ResourceWrapper::sunPositionChanged()
{
    m_shade_valid = false;

    emit resourceInvalidated();
}

TerrainWater & ResourceWrapper::getTerrainWater()
{
    return m_terrain_water;
}

SoilInfiltration & ResourceWrapper::getSoilInfiltration()
{
    return m_soil_infiltration;
}

TerrainDailyIllumination & ResourceWrapper::getDailyIllumination()
{
    return m_terrain_daily_illumination;
}

TerrainShade & ResourceWrapper::getShade()
{
    return m_terrain_shade;
}

TerrainTemperature & ResourceWrapper::getTerrainTemp()
{
    return m_terrain_temp;
}

SoilHumidity & ResourceWrapper::getSoilHumidity()
{
    return m_soil_humidity;
}

WeightedSoilHumidity & ResourceWrapper::getWeightedSoilHumidity()
{
    return m_weighted_soil_humidity;
}

Slope & ResourceWrapper::getSlope()
{
    return m_slope;
}

void ResourceWrapper::getResourceInfo(const glm::vec2 & pos, int month, float & slope, float & water_height, bool & shaded, int & min_illumination,
                                      int & max_illumination, float & temp, int & soil_infiltration_rate, int & soil_humidity, float & weighted_soil_humidity)
{
    // Slope
    {
        slope = m_slope(pos[0], pos[1]);
    }
    // Water
    {
        water_height = m_terrain_water(month-1, pos[0], pos[1]);
    }
    // Shade
    if(m_shade_valid)
        shaded = m_terrain_shade(pos[0], pos[1]);
    // Daily illumination
    if(m_daily_illumination_valid)
    {
        min_illumination = (int) m_terrain_daily_illumination.getMin()(pos[0], pos[1]);
        max_illumination = (int) m_terrain_daily_illumination.getMax()(pos[0], pos[1]);
    }
    // Temperature
    if(m_temp_valid)
    {
        GLbyte jun = m_terrain_temp(TerrainTemperature::_JUN_LAYER_IDX, pos[0], pos[1]);
        GLbyte dec = m_terrain_temp(TerrainTemperature::_DEC_LAYER_IDX, pos[0], pos[1]);

        GLbyte temp_diff = jun - dec;

        float jun_percentage = (6.f-(abs(6.f-month)))/6.f;
        temp = dec + (jun_percentage * temp_diff);
    }
    // Soil infiltration rate
    {
        soil_infiltration_rate = m_soil_infiltration(pos[0],pos[1]);
    }
    // Soil Humidity
    {
        soil_humidity = m_soil_humidity(month-1, pos[0], pos[1]);
    }
    // Weighted soil Humidity
    {
        weighted_soil_humidity = m_weighted_soil_humidity(month-1, pos[0], pos[1]);
    }
}

void ResourceWrapper::refreshShade(Terrain & terrain, const glm::vec3 & sun_position)
{
    m_terrain_shade.setData(get_shade(terrain, sun_position));
    m_shade_valid = true;
}

GLubyte * ResourceWrapper::get_shade(Terrain & terrain, const glm::vec3 & sun_position, bool emit_progress_updates)
{
    if(emit_progress_updates)
    {
        emit processing("Refreshing shade");
    }

    int terrain_width(terrain.getWidth());
    int terrain_depth(terrain.getDepth());
    int n_iterations(terrain_depth*terrain_width);

    TerrainNormals & terrain_normals (terrain.getNormals());

    GLubyte * shade_data = NULL;

    // Generate the data
    shade_data = (GLubyte *) malloc(sizeof(GLubyte) * terrain_width * terrain_depth);

    if(sun_position[1] <= terrain.getMinHeight()) // Optimization - the sun is set. There will be no light
    {
        memset(shade_data, 0xFF, terrain_depth*terrain_width);
    }
    else
    {
        glm::vec3 dummy_intersection_point;
        int i(0);
        for(int z = 0 ; z < terrain_depth; z++, i+=terrain_width)
        {
            if(emit_progress_updates)
            {
                emit processingUpdate((((float)i)/n_iterations)*100);
            }
#pragma omp parallel for
            for(int x = 0; x < terrain_width; x++)
            {
                glm::vec3 point_on_terrain(glm::vec3(x, terrain.getHeight(glm::vec2(x,z)), z));
                glm::vec3 sun_direction(glm::normalize(sun_position - point_on_terrain));

                // First check if the normal is within sun angle
                int index(z*terrain_depth+x);

                if(glm::dot(terrain_normals(x,z), sun_direction) < 0 ||
                        terrain.traceRay(point_on_terrain, sun_direction, dummy_intersection_point, false)) // Not within direction
                {
                    shade_data[index] = 0xFF;
                }
                else
                {
                    shade_data[index] = 0x00;
                }
            }
        }
    }

    if(emit_progress_updates)
    {
        emit processingComplete();
    }

    return shade_data;
}

void ResourceWrapper::refreshDailyIllumination(LightingManager & lighting_manager, Terrain & terrain)
{
    int current_month(lighting_manager.currentMonth());
    int current_time(lighting_manager.currentTime());

    int terrain_width(terrain.getWidth());
    int terrain_depth(terrain.getDepth());
    int n_elements(terrain_width*terrain_depth);

    GLubyte * intermediary_illumination_data = (GLubyte*) malloc(sizeof(GLubyte)*n_elements);
    GLubyte * aggregated_min = (GLubyte*) malloc(sizeof(GLubyte)*n_elements);
    GLubyte * aggregated_max = (GLubyte*) malloc(sizeof(GLubyte)*n_elements);

    float n_iterations (24 * 12);
    int iteration(0);

    emit processing("Calculating daily illumination");

    for(int month = 1; month < 13; month++)
    {
        lighting_manager.setMonth(month);
        memset(intermediary_illumination_data, 0x00, n_elements);

        for(int hour = 0; hour < 24; hour++, iteration++)
        {
            // First update progress dialog
            QString description("Month: ");
            description.append(QString::number(month)).append( " | Hour: ").append(QString::number(hour));
            emit processDescriptionUpdate(description);
            emit processingUpdate((iteration / n_iterations)*100);

            lighting_manager.setTime(hour * 60);
            GLubyte * shade_data = get_shade(terrain, lighting_manager.getSunlightProperties().getPosition(), false) ;
#pragma omp parallel for
            for(int z = 0 ; z < terrain_depth; z++)
            {
                for(int x = 0; x < terrain_width; x++)
                {
                    int index(z*terrain_width+x);
                    intermediary_illumination_data[index] += (shade_data[index] > 0 ? 0 : 1);
                }
            }
            free(shade_data);
        }
        // Now check if it is the minimum/maximum
        if(month > 1)
        {
#pragma omp parallel for
            for(int z = 0 ; z < terrain_depth; z++)
            {
                for(int x = 0; x < terrain_width; x++)
                {
                    int index(z*terrain_width+x);
                    GLubyte value(intermediary_illumination_data[index]);
                    if(value > aggregated_max[index])
                        aggregated_max[index] = value;
                    if(value < aggregated_min[index])
                        aggregated_min[index] = value;
                }
            }
        }
        else
        {
            std::memcpy(aggregated_min, intermediary_illumination_data, n_elements);
            std::memcpy(aggregated_max, intermediary_illumination_data, n_elements);
        }
    }
    free(intermediary_illumination_data);
    m_terrain_daily_illumination.getMin().setData(aggregated_min);
    m_terrain_daily_illumination.getMax().setData(aggregated_max);

    // Restore time and date
    lighting_manager.setMonth(current_month);
    lighting_manager.setTime(current_time);

    emit processingComplete();

    m_daily_illumination_valid = true;
}

void ResourceWrapper::refreshTemperature(const Terrain & terrain, float temp_at_zero_june, float lapse_rate_june, float temp_at_zero_dec, float lapse_rate_dec)
{
    int terrain_width(terrain.getWidth());
    int terrain_depth(terrain.getDepth());
    int n_iterations(terrain_depth*terrain_width*2);

    emit processing("Calculating Temperature...");
    int iteration_counter(0);

//    GLbyte * temp_data = new GLbyte[terrain_width * terrain_depth * 2];
    GLbyte * jun_temp_data = new GLbyte[terrain_width * terrain_depth];
    GLbyte * dec_temp_data = new GLbyte[terrain_width * terrain_depth];

////    std::memset(temp_data, 0, sizeof(GLbyte)*terrain_width * terrain_depth * 2);

//    int dec_start_idx(terrain_width * terrain_depth);

//    GLbyte * data = new GLbyte[terrain_width * terrain_depth * 2];
//    std::memset(data, 0, terrain_width * terrain_depth * 2 * sizeof(GLbyte));

    // June temperatures
    {
        float temp_at_zero(temp_at_zero_june);
        float lapse_rate(lapse_rate_june);

        for(int z = 0 ; z < terrain_depth; z++, iteration_counter += terrain_width)
        {
            emit processingUpdate((((float)iteration_counter)/n_iterations)*100);
#pragma omp parallel for
            for(int x = 0; x < terrain_width; x++)
            {
                int index(z*terrain_width+x);

                float altitude( terrain.getAltitude(glm::vec2(x,z)) );

                float temp(temp_at_zero - ((altitude/1000.0f) * lapse_rate));

                jun_temp_data[index] = std::min(50.0f,std::max(-50.0f, temp));
            }
        }
    }

    // December temperatures
    {
        float temp_at_zero(temp_at_zero_dec);
        float lapse_rate(lapse_rate_dec);

        int i(0);
        for(int z = 0 ; z < terrain_depth; z++, iteration_counter += terrain_width)
        {
            emit processingUpdate((((float)iteration_counter)/n_iterations)*100);
#pragma omp parallel for
            for(int x = 0; x < terrain_width; x++)
            {
                int index((z*terrain_width+x));

                float altitude( terrain.getAltitude(glm::vec2(x,z)) );

                float temp(temp_at_zero - ((altitude/1000.0f) * lapse_rate));
                dec_temp_data[index] = std::min(50.0f,std::max(-50.0f, temp));
            }
        }
    }

//    m_terrain_temp.setData(temp_data, terrain_width, terrain_depth, 2);
    m_terrain_temp.setData(TerrainTemperature::_JUN_LAYER_IDX,jun_temp_data);
    m_terrain_temp.setData(TerrainTemperature::_DEC_LAYER_IDX,dec_temp_data);
//    m_terrain_temp.getDec().setData(dec_temp_data);

//    m_terrain_temp.setData(data);

    emit processingComplete();
    m_temp_valid = true;
}
