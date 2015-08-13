#include "computer.h"
#include "glheader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include <algorithm>

Computer::CachedWaterTexture::CachedWaterTexture() :
    TextureElement2D<GLfloat>(QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32)
{

}

Computer::CachedWaterTexture::~CachedWaterTexture()
{

}

Computer::WaterHeightmapBorderReductionTexture::WaterHeightmapBorderReductionTexture() :
    TextureElement2D<GLfloat>(QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32)
{

}

Computer::WaterHeightmapBorderReductionTexture::~WaterHeightmapBorderReductionTexture()
{

}


Computer::Computer()
{
}

Computer::~Computer()
{

}

void Computer::compileAndLinkShaders()
{
    m_shaders.compileAndLink();
}

ComputeShaderProgram * Computer::get_overlay_creator_shader(const char * active_overlay)
{
    if(active_overlay == Uniforms::Overlay::_SLOPE)
        return &m_shaders.m_overlay_creator_shaders.m_slope;
    else if(active_overlay == Uniforms::Overlay::_ALTITUDE)
        return &m_shaders.m_overlay_creator_shaders.m_altitude;
    else if(active_overlay == Uniforms::Overlay::_SHADE)
        return &m_shaders.m_overlay_creator_shaders.m_shade;
    else if(active_overlay == Uniforms::Overlay::_TEMPERATURE)
        return &m_shaders.m_overlay_creator_shaders.m_temperature;
    else if(active_overlay == Uniforms::Overlay::_MIN_DAILY_ILLUMINATION ||
            active_overlay == Uniforms::Overlay::_MAX_DAILY_ILLUMINATION)
    {
        return &m_shaders.m_overlay_creator_shaders.m_daily_illumination;
    }
    else if(active_overlay == Uniforms::Overlay::_SOIL_INFILTRATION_RATE)
        return &m_shaders.m_overlay_creator_shaders.m_soil_infiltration_rate;
    else if(active_overlay == Uniforms::Overlay::_MONTHLY_SOIL_HUMIDITY)
        return &m_shaders.m_overlay_creator_shaders.m_soil_humidity;
    else if(active_overlay == Uniforms::Overlay::_WEIGHTED_AVG_SOIL_HUMIDITY)
        return &m_shaders.m_overlay_creator_shaders.m_weigted_avg_soil_humidity;

    qCritical() << "Returning no shader. Overlay: " << active_overlay;
}


void Computer::createOverlayTexture(GLuint overlay_texture_id, Terrain & terrain, ResourceWrapper & resources, const char * active_overlay, int month)
{
    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    glm::vec3 group_count ( calculateGroupCount(terrain.getWidth(), terrain.getDepth(), 1,
                                        OverlayTextureCreatorShaders::_GROUP_SIZE_X,
                                        OverlayTextureCreatorShaders::_GROUP_SIZE_Y,
                                        OverlayTextureCreatorShaders::_GROUP_SIZE_Z));


    ComputeShaderProgram * shader = get_overlay_creator_shader(active_overlay);
    shader->bind();
    f->glActiveTexture(GL_TEXTURE0); CE();

    if(active_overlay == Uniforms::Overlay::_ALTITUDE)
    {
        shader->setUniformValue(Uniforms::Texture::_TERRAIN_HEIGHTMAP, 0); CE();
        terrain.getDrawableTerrain().bind();CE();
        shader->setUniformValue(Uniforms::Terrain::_MIN_HEIGHT, (GLfloat)(terrain.getMinHeight())); CE();
        shader->setUniformValue(Uniforms::Terrain::_MAX_HEIGHT, (GLfloat)(terrain.getMaxHeight())); CE();
    }
    else if(active_overlay == Uniforms::Overlay::_SLOPE)
    {
        shader->setUniformValue(Uniforms::Texture::_SLOPE, 0); CE();
        resources.getSlope().bind();
    }
    else if(active_overlay == Uniforms::Overlay::_SHADE)
    {
        shader->setUniformValue(Uniforms::Texture::_SHADE, 0); CE();
        resources.getShade().bind();
    }
    else if(active_overlay == Uniforms::Overlay::_TEMPERATURE)
    {
        shader->setUniformValue(Uniforms::Texture::_TEMPERATURE, 0); CE();
        resources.getTerrainTemp().bind();
        shader->setUniformValue(Uniforms::Timing::_MONTH, month); CE();
    }
    else if(active_overlay == Uniforms::Overlay::_MIN_DAILY_ILLUMINATION)
    {
        shader->setUniformValue(Uniforms::Texture::_DAILY_ILLUMINATION, 0); CE();
        resources.getDailyIllumination()[TerrainDailyIllumination::_MIN_LAYER_IDX]->bind();
    }
    else if(active_overlay == Uniforms::Overlay::_MAX_DAILY_ILLUMINATION)
    {
        shader->setUniformValue(Uniforms::Texture::_DAILY_ILLUMINATION, 0); CE();
        resources.getDailyIllumination()[TerrainDailyIllumination::_MAX_LAYER_IDX]->bind();
    }
    else if(active_overlay == Uniforms::Overlay::_SOIL_INFILTRATION_RATE)
    {
        shader->setUniformValue(Uniforms::Texture::_SOIL_INFILTRATION_RATE, 0); CE();
        resources.getSoilInfiltration().bind();
    }
    else if(active_overlay == Uniforms::Overlay::_MONTHLY_SOIL_HUMIDITY)
    {
        shader->setUniformValue(Uniforms::Texture::_MONTHLY_SOIL_HUMIDITY, 0); CE();
        resources.getSoilHumidity()[month-1]->bind();
    }
    else if(active_overlay == Uniforms::Overlay::_WEIGHTED_AVG_SOIL_HUMIDITY)
    {
        shader->setUniformValue(Uniforms::Texture::_WEIGHTED_AVG_SOIL_HUMIDITY, 0); CE();
        resources.getWeightedSoilHumidity()[month-1]->bind();
    }

    f->glBindImageTexture(0, overlay_texture_id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);  CE();

    f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
    f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

    shader->release();
}


GLuint Computer::balanceWater(PaddedTerrain & padded_terrain,
                              TerrainWater & terrain_water,
                              int month)
{
    /*****************
     * BALANCE WATER *
     *****************/
    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    int terrain_width(terrain_water.width());
    int terrain_depth(terrain_water.height());

    glm::uvec3 group_count (calculateGroupCount(terrain_width, terrain_depth, 1,
                                                WaterFluxGeneratorShader::_GROUP_SIZE_X,
                                                WaterFluxGeneratorShader::_GROUP_SIZE_Y,
                                                WaterFluxGeneratorShader::_GROUP_SIZE_Z));

    // Setup overlapping heightmaps
    {
        int width (terrain_width);
        int height (group_count.y*2);
        m_horizontal_overlaps.reset(width, height);
    }

    {
        int width (group_count.x*2);
        int height (terrain_depth);
        m_vertical_overlaps.reset(width, height);
    }

    {
        int width (group_count.x*2);
        int height (group_count.y*2);
        m_corner_overlaps.reset(width, height);
    }

    // First set the cache terrain water to the current data
    {
        GLfloat * cached_terrain_water = new GLfloat[terrain_width*terrain_depth];
        std::memcpy(cached_terrain_water, terrain_water.getRawData(month-1), terrain_width*terrain_depth*sizeof(GLfloat));
        m_terrain_water_cache.setDimensions(terrain_width, terrain_depth);
        m_terrain_water_cache.setData(cached_terrain_water);
    }

    // Now set the counter texture data
    {
        m_water_comparator_counter.reset(group_count.x, group_count.y);
    }

    m_shaders.m_water_flux_generator.bind();  CE();

    f->glBindImageTexture(0, terrain_water[month-1]->textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);  CE();
    f->glBindImageTexture(1, padded_terrain.textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);  CE();
    f->glBindImageTexture(2, m_horizontal_overlaps.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);  CE();
    f->glBindImageTexture(3, m_vertical_overlaps.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);  CE();
    f->glBindImageTexture(4, m_corner_overlaps.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);  CE();

    f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
    f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

    m_shaders.m_water_flux_generator.release();

    /********************
     * BORDER REDUCTION *
     ********************/
    m_shaders.m_border_overlap_reducer.bind();

    f->glBindImageTexture(0, terrain_water[month-1]->textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);  CE();

    // VERTICAL
    {
        m_shaders.m_border_overlap_reducer.setUniformValue(Uniforms::BorderOverlapReduction::_OVERLAP_MODE, 0);
        group_count = calculateGroupCount(m_vertical_overlaps.width(), m_vertical_overlaps.height(), 1,
                                          BorderOverlapReducer::_GROUP_SIZE_X,
                                          BorderOverlapReducer::_GROUP_SIZE_Y,
                                          BorderOverlapReducer::_GROUP_SIZE_Z);
        f->glBindImageTexture(1, m_vertical_overlaps.textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);  CE();
        f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
        f->glMemoryBarrier(GL_ALL_BARRIER_BITS);   CE();
    }

    // HORIZONTAL
    {
        m_shaders.m_border_overlap_reducer.setUniformValue(Uniforms::BorderOverlapReduction::_OVERLAP_MODE, 1);
        group_count = calculateGroupCount(m_horizontal_overlaps.width(), m_horizontal_overlaps.height(), 1,
                                          BorderOverlapReducer::_GROUP_SIZE_X,
                                          BorderOverlapReducer::_GROUP_SIZE_Y,
                                          BorderOverlapReducer::_GROUP_SIZE_Z);
        f->glBindImageTexture(1, m_horizontal_overlaps.textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);  CE();
        f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
        f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();
    }

    // CORNERS
    {
        m_shaders.m_border_overlap_reducer.setUniformValue(Uniforms::BorderOverlapReduction::_OVERLAP_MODE, 2);
        group_count = calculateGroupCount(m_corner_overlaps.width(), m_corner_overlaps.height(), 1,
                                          BorderOverlapReducer::_GROUP_SIZE_X,
                                          BorderOverlapReducer::_GROUP_SIZE_Y,
                                          BorderOverlapReducer::_GROUP_SIZE_Z);
        f->glBindImageTexture(1, m_corner_overlaps.textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);  CE();
        f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
        f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();
    }

    m_shaders.m_border_overlap_reducer.release();

    /*****************
     * BALANCE CHECK *
     *****************/
    group_count = calculateGroupCount(terrain_width, terrain_depth, 1,
                                      WaterComparatorShader::_GROUP_SIZE_X,
                                      WaterComparatorShader::_GROUP_SIZE_Y,
                                      WaterComparatorShader::_GROUP_SIZE_Z);
    m_shaders.m_water_comparator.bind();
    f->glBindImageTexture(0, m_terrain_water_cache.textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);  CE();
    f->glBindImageTexture(1, terrain_water[month-1]->textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);  CE();
    f->glBindImageTexture(2, m_water_comparator_counter.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);  CE();

    f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
    f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

    m_shaders.m_water_comparator.release();

    m_water_comparator_counter.syncFromGPU();
    GLuint aggregated_verticies_changed(0);
    for(int x = 0; x < group_count.x; x++)
    {
        for(int y = 0; y < group_count.y; y++)
        {
            aggregated_verticies_changed += m_water_comparator_counter(x, y);
        }
    }

    return aggregated_verticies_changed;
}

void Computer::calculateSoilHumidityAndStandingWater(GLuint soil_infiltration_texture_id,
                                                     GLuint resulting_soil_humidity_texture_id,
                                                     GLuint resulting_standing_water_texture_id,
                                                     int rainfall,
                                                     int rainfall_intensity,
                                                     int terrain_width,
                                                     int terrain_depth,
                                                     float terrain_scale)
{
    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    glm::uvec3 group_count (calculateGroupCount(terrain_width, terrain_depth, 1,
                                                SoilHumidityCalculatorShader::_GROUP_SIZE_X,
                                                SoilHumidityCalculatorShader::_GROUP_SIZE_Y,
                                                SoilHumidityCalculatorShader::_GROUP_SIZE_Z));

    m_shaders.m_soil_humidity_calculator.bind();
    m_shaders.m_soil_humidity_calculator.setUniformValue(Uniforms::Rainfall::_RAINFALL, rainfall);
    m_shaders.m_soil_humidity_calculator.setUniformValue(Uniforms::Rainfall::_INTENSITY, rainfall_intensity);
    m_shaders.m_soil_humidity_calculator.setUniformValue(Uniforms::Terrain::_SCALE, terrain_scale);

    f->glBindImageTexture(0, soil_infiltration_texture_id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);  CE();
    f->glBindImageTexture(1, resulting_soil_humidity_texture_id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);  CE();
    f->glBindImageTexture(2, resulting_standing_water_texture_id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);  CE();

    f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
    f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

    m_shaders.m_soil_humidity_calculator.release();
}

void Computer::calculateWeightedSoilHumidity(SoilHumidity & soil_humdities,
                                             WeightedSoilHumidity & weighted_soil_humidities)
{
    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    glm::uvec3 group_count (calculateGroupCount(soil_humdities.width(), soil_humdities.height(), 1,
                                                WeightedAverageCalculator::_GROUP_SIZE_X,
                                                WeightedAverageCalculator::_GROUP_SIZE_Y,
                                                WeightedAverageCalculator::_GROUP_SIZE_Z));

    m_shaders.m_weighted_avg_calculator.bind();

    for(int i(0); i < 12; i++)
    {
        f->glBindImageTexture(0, soil_humdities[i]->textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);  CE(); // Soil humidity
        f->glBindImageTexture(1, soil_humdities[((i+11) % 12)]->textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);  CE(); // Soil humidity
        f->glBindImageTexture(2, soil_humdities[((i+10) % 12)]->textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);  CE(); // Soil humidity

        f->glBindImageTexture(3, weighted_soil_humidities[i]->textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);  CE(); // Resulting weighted soil humidity

        f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
        f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();
    }

    m_shaders.m_weighted_avg_calculator.release();
}

void Computer::slopeBasedInfiltrationRateFilter(Slope & slope,
                                                SoilInfiltration & soil_infiltration,
                                                int min_slope)
{
    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    glm::uvec3 group_count (calculateGroupCount(slope.width(), slope.height(), 1,
                                                SlopeBasedSoilInfiltrationShader::_GROUP_SIZE_X,
                                                SlopeBasedSoilInfiltrationShader::_GROUP_SIZE_Y,
                                                SlopeBasedSoilInfiltrationShader::_GROUP_SIZE_Z));

    m_shaders.m_slope_based_soil_infiltration_calculator.bind();

    m_shaders.m_slope_based_soil_infiltration_calculator.setUniformValue(Uniforms::SlopeBasedSoilInfiltration::_MIN_SLOPE, min_slope);

    f->glBindImageTexture(0, slope.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);  CE();
    f->glBindImageTexture(1, soil_infiltration.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);  CE();

    f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
    f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

     m_shaders.m_slope_based_soil_infiltration_calculator.release();
}


void Computer::setAbsoluteAggregateHeight(Terrain & terrain,
                                GLuint terrain_water_texture_id,
                                float height)
{
    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    glm::uvec3 group_count (calculateGroupCount(terrain.getWidth(), terrain.getDepth(), 1,
                                                AggregateHeightShader::_GROUP_SIZE_X,
                                                AggregateHeightShader::_GROUP_SIZE_Y,
                                                AggregateHeightShader::_GROUP_SIZE_Z));

    m_shaders.m_aggregate_height.bind();CE();

    m_shaders.m_aggregate_height.setUniformValue(Uniforms::WaterEdit::_ABSOLUTE_HEIGHT, height);CE();
    m_shaders.m_aggregate_height.setUniformValue(Uniforms::Terrain::_SCALE, terrain.getScale());CE();

    f->glBindImageTexture(0, terrain_water_texture_id, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);  CE();
    f->glBindImageTexture(1, terrain.getDrawableTerrain().textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);  CE();

    f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
    f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

    m_shaders.m_aggregate_height.release();
}

void Computer::convertNormalsToSlope(TerrainNormals & terrain_normals, Slope & slope)
{
    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    glm::uvec3 group_count (calculateGroupCount(slope.width(), slope.height(), 1,
                                                AggregateHeightShader::_GROUP_SIZE_X,
                                                AggregateHeightShader::_GROUP_SIZE_Y,
                                                AggregateHeightShader::_GROUP_SIZE_Z));

    m_shaders.m_normals_to_slope_converter.bind(); CE();

    // The Normals texture
    f->glActiveTexture(GL_TEXTURE0);CE();
    m_shaders.m_normals_to_slope_converter.setUniformValue(Uniforms::Texture::_NORMALS, 0); CE();
    f->glBindTexture(GL_TEXTURE_2D, terrain_normals.getTextureUnit());CE();

    // The resulting slope
    f->glBindImageTexture(1, slope.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);  CE();

    f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
    f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

    m_shaders.m_normals_to_slope_converter.release();
}

void Computer::findClosestCluster(Clusters & clusters, ResourceWrapper & resources, ClusterMembershipTexture & cluster_memberships)
{
    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    glm::uvec3 group_count (calculateGroupCount(resources.getSlope().width(), resources.getSlope().height(), 1,
                                                AggregateHeightShader::_GROUP_SIZE_X,
                                                AggregateHeightShader::_GROUP_SIZE_Y,
                                                AggregateHeightShader::_GROUP_SIZE_Z));

    m_shaders.m_closest_cluster_finder.bind();

    /****************
     * SET UNIFORMS *
     ****************/
    // # clusters
    m_shaders.m_closest_cluster_finder.setUniformValue(Uniforms::Clustering::_N_CLUSTERS, clusters.clusterCount());
    // Temp
    m_shaders.m_closest_cluster_finder.setUniformValueArray(Uniforms::Clustering::_JUN_TEMPERATURES, clusters.m_jun_temperature, Clusters::_MAX_CLUSTERS);
    m_shaders.m_closest_cluster_finder.setUniformValueArray(Uniforms::Clustering::_DEC_TEMPERATURES, clusters.m_dec_temperature, Clusters::_MAX_CLUSTERS);
    // Illumination
    m_shaders.m_closest_cluster_finder.setUniformValueArray(Uniforms::Clustering::_MIN_ILLUMINATION, clusters.m_min_illuminations, Clusters::_MAX_CLUSTERS);
    m_shaders.m_closest_cluster_finder.setUniformValueArray(Uniforms::Clustering::_MAX_ILLUMINATION, clusters.m_max_illuminations, Clusters::_MAX_CLUSTERS);
    // Slope
    m_shaders.m_closest_cluster_finder.setUniformValueArray(Uniforms::Clustering::_SLOPE, clusters.m_slopes, Clusters::_MAX_CLUSTERS, 1);
    // Soil humidity
    for(int i (0); i < 12; i++)
    {
        m_shaders.m_closest_cluster_finder.setUniformValueArray(Uniforms::Clustering::_SOIL_HUMIDITIES[i], clusters.m_soil_humidities[i],
                                                                Clusters::_MAX_CLUSTERS, 1);
    }

    /****************
     * SET TEXTURES *
     ****************/
    int texture_unit(GL_TEXTURE0); // TEXTURE_0 reserved for the heightmap
//    // June temperature
//    {
//        f->glActiveTexture(texture_unit);CE();
//        m_shaders.m_closest_cluster_finder.setUniformValue(Uniforms::Texture::_TEMPERATURE_JUN, texture_unit-GL_TEXTURE0); CE();
//        resources.getTerrainTemp().getJun().bind(); CE();
//        texture_unit++;
//    }
//    // December Temp
//    {
//        f->glActiveTexture(texture_unit);CE();
//        m_shaders.m_closest_cluster_finder.setUniformValue(Uniforms::Texture::_TEMPERATURE_DEC, texture_unit-GL_TEXTURE0); CE();
//        resources.getTerrainTemp().getDec().bind(); CE();
//        texture_unit++;
//    }
    // Min illumination
//    {
//        f->glActiveTexture(texture_unit);CE();
//        m_shaders.m_closest_cluster_finder.setUniformValue(Uniforms::Texture::_MIN_DAILY_ILLUMINATION, texture_unit-GL_TEXTURE0); CE();
//        resources.getDailyIllumination().getMin().bind();CE();
//        texture_unit++;
//    }
//    // Max illumination
//    {
//        f->glActiveTexture(texture_unit);CE();
//        m_shaders.m_closest_cluster_finder.setUniformValue(Uniforms::Texture::_MAX_DAILY_ILLUMINATION, texture_unit-GL_TEXTURE0); CE();
//        resources.getDailyIllumination().getMax().bind();CE();
//        texture_unit++;
//    }
    // Slope
    {
        f->glActiveTexture(texture_unit);CE();
        m_shaders.m_closest_cluster_finder.setUniformValue(Uniforms::Texture::_SLOPE, texture_unit-GL_TEXTURE0); CE();
        resources.getSlope().bind();
        texture_unit++;
    }
    // WEIGHTED SOIL ILLUMINATION
    {
//        for(int i (0); i < 12; i++)
//        {
//            f->glActiveTexture(texture_unit);CE();
//            m_shaders.m_closest_cluster_finder.setUniformValue(Uniforms::Texture::_WEIGHTED_AVG_SOIL_HUMIDITIES[i], texture_unit-GL_TEXTURE0); CE();
//            resources.getWeightedSoilHumidity()[i+1].bind();
//            texture_unit++;
//        }
    }

    // The resulting cluster membership
    f->glBindImageTexture(0, cluster_memberships.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);  CE();

    f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
    f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

    m_shaders.m_closest_cluster_finder.release();
}

glm::uvec3 Computer::calculateGroupCount(int n_threads_x, int n_threads_y, int n_threads_z,
                                         int group_size_x, int group_size_y, int group_size_z)
{
    GLuint n_groups_x(std::max(1, (int) std::ceil(((float)n_threads_x) / group_size_x)));
    GLuint n_groups_y(std::max(1, (int) std::ceil(((float)n_threads_y)/ group_size_y)));
    GLuint n_groups_z(std::max(1, (int) std::ceil(((float)n_threads_z)/ group_size_z)));

    return glm::uvec3(n_groups_x, n_groups_y, n_groups_z);
}
