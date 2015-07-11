#include "resource_wrapper.h"
#include "../lighting/lighting_manager.h"
#include "../terrain/terrain.h"
#include <cstring>

ResourceWrapper::ResourceWrapper()
{

}

ResourceWrapper::~ResourceWrapper()
{

}

void ResourceWrapper::valid(bool & shade, bool & daily_illumination, bool & temp)
{
    shade = m_terrain_shade.isValid();
    daily_illumination = m_terrain_daily_illumination.isValid();
    temp = m_terrain_temp.isValid();
}

void ResourceWrapper::syncTextures()
{
    m_terrain_shade.pushToGPU();
    m_terrain_daily_illumination.pushToGPU();
    m_terrain_temp.pushToGPU();
    m_terrain_water.pushToGPU();
}

void ResourceWrapper::terrainChanged()
{
    m_terrain_shade.invalidate();
    m_terrain_daily_illumination.invalidate();
    m_terrain_temp.invalidate();
//    m_terrain_water.invalidate(); TERRAIN WATER DEALT WITH SEPARATELY

    emit resourceInvalidated();
}

void ResourceWrapper::latitudeChanged()
{
    m_terrain_shade.invalidate();
    m_terrain_daily_illumination.invalidate();

    emit resourceInvalidated();
}

void ResourceWrapper::sunPositionChanged()
{
    m_terrain_shade.invalidate();

    emit resourceInvalidated();
}

void ResourceWrapper::bindShade()
{
    m_terrain_shade.bind();
}

void ResourceWrapper::bindMinIllumination()
{
    m_terrain_daily_illumination.bindMin();
}

void ResourceWrapper::bindMaxIllumination()
{
    m_terrain_daily_illumination.bindMax();
}

void ResourceWrapper::bindJunTemperature()
{
    m_terrain_temp.bindJun();
}

void ResourceWrapper::bindDecTemperature()
{
    m_terrain_temp.bindDec();
}

void ResourceWrapper::bindJunWater()
{
    m_terrain_water.bindJun();
}

void ResourceWrapper::bindDecWater()
{
    m_terrain_water.bindDec();
}

void ResourceWrapper::getResourceInfo(const glm::vec2 & pos, int month, int & water_height, bool & shaded, int & min_illumination, int & max_illumination, float & temp)
{
    if(m_terrain_water.isValid())
        water_height = 1;
    if(m_terrain_shade.isValid())
        shaded = m_terrain_shade(pos[0], pos[1]);
    if(m_terrain_daily_illumination.isValid())
    {
        GLubyte min, max;
        m_terrain_daily_illumination.getIlluminationData(pos[0], pos[1], min, max);
        min_illumination = (int) min;
        max_illumination = (int) max;
    }
    if(m_terrain_temp.isValid())
    {
        GLbyte jun, dec;
        m_terrain_temp.getTempData(pos[0], pos[1], jun, dec);
        GLbyte temp_diff = jun - dec;

        float jun_percentage = (6.f-(abs(6.f-month)))/6.f;
        temp = dec + (jun_percentage * temp_diff);
    }
}

void ResourceWrapper::refreshShade(Terrain & terrain, const glm::vec3 & sun_position)
{
    emit processing("Refreshing shade");

    int terrain_width(terrain.getWidth());
    int terrain_depth(terrain.getDepth());
    int n_iterations(terrain_depth*terrain_width);

    TerrainNormals & terrain_normals (terrain.getNormals());

    GLubyte * shade_data = NULL;

    if(!m_terrain_shade.isValid())    {
        // Generate the data
        shade_data = (GLubyte *) malloc(sizeof(GLubyte) * terrain_width * terrain_depth);

        if(sun_position[1] <= terrain.getBaseHeight()) // Optimization - the sun is set. There will be no light
        {
            memset(shade_data, 0xFF, terrain_depth*terrain_width);
        }
        else
        {
            glm::vec3 dummy_intersection_point;
            int i(0);
            for(int z = 0 ; z < terrain_depth; z++, i+=terrain_width)
            {
                emit processingUpdate((((float)i)/n_iterations)*100);
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

        m_terrain_shade.setData(shade_data, terrain_width, terrain_depth);
    }

    emit processingComplete();
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

    for(int month = 1; month < 13; month++)
    {
        lighting_manager.setMonth(month);
        memset(intermediary_illumination_data, 0x00, n_elements);

        for(int hour = 0; hour < 24; hour++)
        {
            lighting_manager.setTime(hour * 60);
            refreshShade(terrain, lighting_manager.getSunlightProperties().getPosition() );
            const GLubyte * shade_data(m_terrain_shade.getRawData());
#pragma omp parallel for
            for(int z = 0 ; z < terrain_depth; z++)
            {
                for(int x = 0; x < terrain_width; x++)
                {
                    int index(z*terrain_width+x);
                    intermediary_illumination_data[index] += (shade_data[index] > 0 ? 0 : 1);
                }
            }
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
    m_terrain_daily_illumination.setData(aggregated_min, aggregated_max, terrain_width, terrain_depth);

    // Restore time and date
    lighting_manager.setMonth(current_month);
    lighting_manager.setTime(current_time);
}

void ResourceWrapper::refreshTemperature(const Terrain & terrain, float temp_at_zero_june, float lapse_rate_june, float temp_at_zero_dec, float lapse_rate_dec)
{
    int terrain_width(terrain.getWidth());
    int terrain_depth(terrain.getDepth());
    int n_iterations(terrain_depth*terrain_width*2);

    emit processing("Calculating Temperature...");
    int iteration_counter(0);

    GLbyte * jun_temp_data = (GLbyte*) malloc(sizeof(GLbyte) * terrain_width * terrain_depth);
    GLbyte * dec_temp_data = (GLbyte*) malloc(sizeof(GLbyte) * terrain_width * terrain_depth);

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
                int index(z*terrain_depth+x);

                float altitude( terrain.getAltitude(glm::vec2(x,z)) );

                float temp(temp_at_zero - ((altitude/1000.0f) * lapse_rate));

                jun_temp_data[index] = (GLbyte) std::min(50.0f,std::max(-50.0f, temp));
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
                int index(z*terrain_depth+x);

                float altitude( terrain.getAltitude(glm::vec2(x,z)) );

                float temp(temp_at_zero - ((altitude/1000.0f) * lapse_rate));

                dec_temp_data[index] = (GLbyte) std::min(50.0f,std::max(-50.0f, temp));
            }
        }
    }
    m_terrain_temp.setData(jun_temp_data, dec_temp_data, terrain_width, terrain_depth);

    emit processingComplete();
}

void ResourceWrapper::refreshWater(int terrain_width, int terrain_depth, int rainfall_jun, int rainfal_intensity_jun, int rainfall_dec, int rainfal_intensity_dec)
{
    int sz(sizeof(GLuint) * terrain_width * terrain_depth);

    GLuint * jun_water_data = (GLuint*) malloc(sz);
    GLuint * dec_water_data = (GLuint*) malloc(sz);

    if( rainfall_jun == 0 && rainfall_dec == 0 ) // Optimization
    {
        std::memset(jun_water_data, 0, sz);
        std::memset(dec_water_data, 0, sz);
    }
    else
    {
#pragma omp parallel for
        for(int z = 0; z < terrain_depth; z++)
        {
#pragma omp parallel for
            for(int x = 0; x < terrain_width; x++)
            {
                int index(z*terrain_depth+x);
                jun_water_data[index] = rainfall_jun;
                dec_water_data[index] = rainfall_dec;
            }
        }
    }

    m_terrain_water.setData(jun_water_data, dec_water_data, terrain_width, terrain_depth);

    if( rainfall_jun == 0 && rainfall_dec == 0 ) // No point trying to balance
        m_terrain_water.setBalanced(true);
}

