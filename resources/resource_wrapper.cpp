#include "resource_wrapper.h"
#include "../lighting/lighting_manager.h"
#include "../terrain/terrain.h"
#include <cstring>

#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

ResourceWrapper::ResourceWrapper() :
    m_temp_valid(false),
    m_daily_illumination_valid(false),
    m_shade_valid(false)
{
    connect(&m_terrain_water, SIGNAL(water_balanced(int)), this, SLOT(emit_water_balanced(int)));
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

    m_terrain_daily_illumination.setDimensions(width, depth);

    m_terrain_temp.setDimensions(width, depth);

    m_terrain_water.setDimensions(width, depth);

    m_soil_humidity.setDimensions(width, depth);

    m_weighted_soil_humidity.setDimensions(width, depth);

    m_soil_infiltration.reset(width, depth);

    m_slope.setDimensions(width, depth);

    m_terrain_shade.setDimensions(width, depth);

    setTempValid(false);
    setDailyIlluminationValid(false);
    setShadeValid(false);
}

void ResourceWrapper::latitudeChanged()
{
    setShadeValid(false);
    setDailyIlluminationValid(false);
}

void ResourceWrapper::sunPositionChanged()
{
    setShadeValid(false);
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

void ResourceWrapper::getResourceInfo(const glm::vec2 & pos, int month, float & slope, float & water_height, bool & shaded, int & illumination,
                                      int & temp, int & soil_infiltration_rate, int & soil_humidity, float & weighted_soil_humidity)
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
        illumination = (int) m_terrain_daily_illumination(month-1, pos[0], pos[1]);
    // Temperature
    if(m_temp_valid)
        temp = (int) m_terrain_temp(month-1, pos[0], pos[1]);
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
    setShadeValid(true);
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
    shade_data = new GLubyte[terrain_width * terrain_depth];

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

    float n_iterations (24 * 12);
    int iteration(0);

    emit processing("Calculating daily illumination");

    for(int month = 1; month < 13; month++)
    {
        GLubyte * illumination_data = new GLubyte[n_elements];
        memset(illumination_data, 0, n_elements*sizeof(GLubyte));

        lighting_manager.setMonth(month);

        for(int hour = 0; hour < 24; hour++, iteration++)
        {
            // First update progress dialog
            QString description("Calculating daily illumination for: ");
            description.append(QString::number(month)).append( " @ ").append(QString::number(hour)).append("h.");
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
                    illumination_data[index] += (shade_data[index] > 0 ? 0 : 1);
                }
            }
            delete shade_data;
        }
        // Set data in texture
        m_terrain_daily_illumination.setData(month-1, illumination_data);

        // Calculate min max
        int min(m_terrain_daily_illumination(month-1, 0, 0)), max(m_terrain_daily_illumination(month-1, 0, 1));

        for(int x(0); x < m_terrain_daily_illumination.width(); x++)
            for(int y(0); y < m_terrain_daily_illumination.height(); y++)
            {
                int v(m_terrain_daily_illumination(month-1, x, y) );
                if(v < min)
                    min = v;
                if(v > max)
                    max = v;
            }

        m_terrain_daily_illumination.setMin(month, min);
        m_terrain_daily_illumination.setMax(month, max);
    }

    // Restore time and date
    lighting_manager.setMonth(current_month);
    lighting_manager.setTime(current_time);

    emit processingComplete();

    setDailyIlluminationValid(true);
}

void ResourceWrapper::refreshTemperature(const Terrain & terrain, int temp_at_zero_june, int temp_at_zero_dec, float lapse_rate)
{    
    int terrain_width(terrain.getWidth());
    int terrain_depth(terrain.getDepth());
    int n_iterations(terrain_depth*terrain_width*12);

    emit processing("Calculating Temperature...");
    int iteration_counter(0);

    int temp_diff(temp_at_zero_june-temp_at_zero_dec);

    for(int month = 1; month <= 12; month++)
    {
        GLbyte * temp_data = new GLbyte[terrain_width * terrain_depth];

        // Calculate base temp
        float jun_percentage ( (6-(abs(6-month)))/6.f );
        float temp_at_zero( temp_at_zero_dec + (jun_percentage * temp_diff) );

        for(int z = 0 ; z < terrain_depth; z++, iteration_counter += terrain_width)
        {
            emit processingUpdate((((float)iteration_counter)/n_iterations)*100);
#pragma omp parallel for
            for(int x = 0; x < terrain_width; x++)
            {
                int index(z*terrain_width+x);

                float altitude( terrain.getAltitude(glm::vec2(x,z)) );

                float temp(temp_at_zero - ((altitude/1000.0f) * lapse_rate));

                temp_data[index] = (GLbyte) std::round(std::min(50.0f,std::max(-50.0f, temp)));
            }
        }
        m_terrain_temp.setData(month-1,temp_data);

        // Set the min and max
        {
            float min_altitude( terrain.getMinAltitude());
            float max_temp(temp_at_zero - ((min_altitude/1000.0f) * lapse_rate));
            m_terrain_temp.setMax(month, max_temp);
        }
        {
            float max_altitude( terrain.getMaxAltitude());
            float min_temp(temp_at_zero - ((max_altitude/1000.0f) * lapse_rate));
            m_terrain_temp.setMin(month, min_temp);
        }
    }

    emit processingComplete();
    setTempValid(true);
}

void ResourceWrapper::check_if_all_resources_valid()
{
    if(m_temp_valid && m_daily_illumination_valid)
        emit tempAndDailyIlluminationValid();
}

void ResourceWrapper::setTempValid(bool valid)
{
    m_temp_valid = valid;

    if(valid)
    {
        check_if_all_resources_valid();
    }
    else
        emit tempInvalidated();
}

void ResourceWrapper::setDailyIlluminationValid(bool valid)
{
    m_daily_illumination_valid = valid;

    if(valid)
    {
        check_if_all_resources_valid();
    }
    else
        emit dailyIlluminationInvalidated();
}

void ResourceWrapper::setShadeValid(bool valid)
{
    m_shade_valid = valid;

    if(!valid)
        emit shadeInvalidated();
}

void ResourceWrapper::emit_water_balanced(int month)
{
    emit water_balanced(month);
}
