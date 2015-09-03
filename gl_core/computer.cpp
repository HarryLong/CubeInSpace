#include "computer.h"
#include "glheader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include <algorithm>

// CACHED WATER TEXTURE
Computer::CachedWaterTexture::CachedWaterTexture() :
    TextureElement2D<GLfloat>(QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32)
{

}

Computer::CachedWaterTexture::~CachedWaterTexture()
{

}
//-----------------------------
// WATER HEIGHTMAP BORDER REDUCTION TEXTURE
Computer::WaterHeightmapBorderReductionTexture::WaterHeightmapBorderReductionTexture() :
    TextureElement2D<GLfloat>(QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32)
{

}

Computer::WaterHeightmapBorderReductionTexture::~WaterHeightmapBorderReductionTexture()
{

}
//------------------------------
// SLOPE AND HUMIDITY CLUSTER REDUCTION TEXTURE
Computer::SlopeAndHumidityClusterReductionTexture::SlopeAndHumidityClusterReductionTexture() :
    TextureElement2DArray<GLfloat>(QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32)
{

}

Computer::SlopeAndHumidityClusterReductionTexture::~SlopeAndHumidityClusterReductionTexture()
{

}
//------------------------------
// TEMPERATURE CLUSTER REDUCTION TEXTURE
Computer::TemperatureClusterReductionTexture::TemperatureClusterReductionTexture() :
    TextureElement2DArray<GLfloat>(QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32)
{

}
Computer::TemperatureClusterReductionTexture::~TemperatureClusterReductionTexture()
{

}
//-----------------------------
// DAILY ILLUMINATION CLUSTER REDUCTION TEXTURE
Computer::DailyIlluminationClusterReductionTexture::DailyIlluminationClusterReductionTexture() :
    TextureElement2DArray<GLfloat>(QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32)
{

}

Computer::DailyIlluminationClusterReductionTexture::~DailyIlluminationClusterReductionTexture()
{

}

//-------------------------------
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
    else if(active_overlay == Uniforms::Overlay::_ILLUMINATION)
        return &m_shaders.m_overlay_creator_shaders.m_daily_illumination;
    else if(active_overlay == Uniforms::Overlay::_SOIL_INFILTRATION_RATE)
        return &m_shaders.m_overlay_creator_shaders.m_soil_infiltration_rate;
    else if(active_overlay == Uniforms::Overlay::_MONTHLY_SOIL_HUMIDITY)
        return &m_shaders.m_overlay_creator_shaders.m_soil_humidity;
    else if(active_overlay == Uniforms::Overlay::_WEIGHTED_AVG_SOIL_HUMIDITY)
        return &m_shaders.m_overlay_creator_shaders.m_weigted_avg_soil_humidity;
    else if(active_overlay == Uniforms::Overlay::_CLUSTERS)
        return &m_shaders.m_overlay_creator_shaders.m_clusters;

    qCritical() << "Returning no shader. Overlay: " << active_overlay;
}


void Computer::createOverlayTexture(GLuint overlay_texture_id,
                                    Terrain & terrain,
                                    ResourceWrapper & resources,
                                    ClusterMembershipTexture & cluster_membership_texture,
                                    const char * active_overlay,
                                    int month)
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
        resources.getTerrainTemp()[month-1]->bind();
    }
    else if(active_overlay == Uniforms::Overlay::_ILLUMINATION)
    {
        shader->setUniformValue(Uniforms::Texture::_DAILY_ILLUMINATION, 0); CE();
        resources.getDailyIllumination()[month-1]->bind();
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
    else if(active_overlay == Uniforms::Overlay::_CLUSTERS)
    {
        qCritical() << "Creating clusteer membership texture";
        shader->setUniformValue(Uniforms::Texture::_CLUSTER_MEMBERSHIP, 0); CE();
        cluster_membership_texture.bind();
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

void Computer::calculateStandingWater(ResourceWrapper & resources,
                                      int month,
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

    m_shaders.m_standing_water_calculator.bind();
    m_shaders.m_standing_water_calculator.setUniformValue(Uniforms::Rainfall::_RAINFALL, rainfall);
    m_shaders.m_standing_water_calculator.setUniformValue(Uniforms::Rainfall::_INTENSITY, rainfall_intensity);
    m_shaders.m_standing_water_calculator.setUniformValue(Uniforms::Terrain::_SCALE, terrain_scale);

    f->glBindImageTexture(0, resources.getSoilInfiltration().textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);  CE();
//    f->glBindImageTexture(1, resulting_soil_humidity_texture_id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);  CE();
    f->glBindImageTexture(1, resources.getTerrainWater()[month-1]->textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);  CE();

    f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
    f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

    m_shaders.m_standing_water_calculator.release();
}

void Computer::calculateSoilHumidity(ResourceWrapper & resources,
                           int month,
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

    f->glBindImageTexture(0, resources.getSoilInfiltration().textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);  CE();
    f->glBindImageTexture(1, resources.getSoilHumidity()[month-1]->textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);  CE();
    f->glBindImageTexture(2, resources.getTerrainWater()[month-1]->textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);  CE();

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

void Computer::kMeansCluster(Clusters & clusters, ResourceWrapper & resources, ClusterMembershipTexture & cluster_memberships, int clustering_iterations)
{
    qCritical() << "Finding closest cluster";

    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    int width(resources.getSlope().width());
    int height(resources.getSlope().height());

    glm::uvec3 closest_cluster_group_count = calculateGroupCount(width, height, 1, KMeansClustererShaders::ClosestClusterFinder::_GROUP_SIZE_X,
                                                                                   KMeansClustererShaders::ClosestClusterFinder::_GROUP_SIZE_Y,
                                                                                   KMeansClustererShaders::ClosestClusterFinder::_GROUP_SIZE_Z);

    glm::uvec3 means_recalculator_group_count = calculateGroupCount(clusters.clusterCount(), 1, 1, KMeansClustererShaders::MeansRecalculator::_GROUP_SIZE_X,
                                                                                                   KMeansClustererShaders::MeansRecalculator::_GROUP_SIZE_Y,
                                                                                                   KMeansClustererShaders::MeansRecalculator::_GROUP_SIZE_Z);

    // Reset size of reduction textures
    m_slope_and_humidity_cluster_reduction.setDimensions(clusters.clusterCount(), closest_cluster_group_count.x*13, closest_cluster_group_count.y);
    m_temperature_cluster_reduction.setDimensions(clusters.clusterCount(), closest_cluster_group_count.x*2, closest_cluster_group_count.y);
    m_daily_illumination_cluster_reduction.setDimensions(clusters.clusterCount(), closest_cluster_group_count.x*12, closest_cluster_group_count.y);

    for(int cluster_iteration(0); cluster_iteration < clustering_iterations; cluster_iteration++)
    {
        /****************************************
         * FIND CLOSEST CLUSTERS FOR EACH POINT *
         ****************************************/
        {
            m_shaders.m_k_means_clusterer.m_closest_cluster_finder.bind();

            m_shaders.m_k_means_clusterer.m_closest_cluster_finder.setUniformValue(Uniforms::Clustering::_N_CLUSTERS, clusters.clusterCount());
            m_shaders.m_k_means_clusterer.m_closest_cluster_finder.setUniformValue(Uniforms::Clustering::_FINAL_CLUSTERING_ITERATION, cluster_iteration == clustering_iterations-1);

            // The resulting cluster membership
            f->glBindImageTexture(0, cluster_memberships.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);  CE();
            /*****************************
             * SET CLUSTER DATA TEXTURES *
             *****************************/
            f->glBindImageTexture(1, clusters.m_slope_cluster_data.textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);  CE();
            // TEMPERATURE
            f->glBindImageTexture(2, clusters.m_temperature_cluster_data[5]->textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R8_SNORM);  CE();
            // DAILY ILLUMINATION
            f->glBindImageTexture(3, clusters.m_daily_illumination_cluster_data.textureId(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_R8);  CE();
            // WEIGHTED SOIL HUMIDITY
            f->glBindImageTexture(4, clusters.m_weighted_soil_humidity_cluster_data.textureId(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32F);  CE();

            /**********************************
             * SET CLUSTER REDUCTION TEXTURES *
             **********************************/
            // SLOPE AND SOIL HUMUDITY
            f->glBindImageTexture(5, m_slope_and_humidity_cluster_reduction.textureId(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32F);  CE();
            // TEMPERATURE
            f->glBindImageTexture(6, m_temperature_cluster_reduction.textureId(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32F);  CE();
            // SLOPE AND SOIL HUMUDITY
            f->glBindImageTexture(7, m_daily_illumination_cluster_reduction.textureId(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32F);  CE();

            /*************************
             * SET RESOURCE TEXTURES *
             *************************/
            int texture_unit(GL_TEXTURE0);
            // Slope
            {
                f->glActiveTexture(texture_unit); CE();
                m_shaders.m_k_means_clusterer.m_closest_cluster_finder.setUniformValue(Uniforms::Texture::_SLOPE, texture_unit-GL_TEXTURE0); CE();
                resources.getSlope().bind() ;CE();
                texture_unit++;
            }
            // Temperature
            {
                f->glActiveTexture(texture_unit); CE();
                m_shaders.m_k_means_clusterer.m_closest_cluster_finder.setUniformValue(Uniforms::Texture::_TEMPERATURE, texture_unit-GL_TEXTURE0); CE();
                resources.getTerrainTemp()[5]->bind() ;CE();
                texture_unit++;
            }
            // Daily illumination
            {
                f->glActiveTexture(texture_unit); CE();
                m_shaders.m_k_means_clusterer.m_closest_cluster_finder.setUniformValue(Uniforms::Texture::_DAILY_ILLUMINATION, texture_unit-GL_TEXTURE0); CE();
                resources.getDailyIllumination().bind() ;CE();
                texture_unit++;
            }
            // Weighted soil humidity
            {
                f->glActiveTexture(texture_unit); CE();
                m_shaders.m_k_means_clusterer.m_closest_cluster_finder.setUniformValue(Uniforms::Texture::_WEIGHTED_AVG_SOIL_HUMIDITY, texture_unit-GL_TEXTURE0); CE();
                resources.getWeightedSoilHumidity().bind() ;CE();
                texture_unit++;
            }

            // COMPUTE
            f->glDispatchCompute(closest_cluster_group_count.x, closest_cluster_group_count.y, closest_cluster_group_count.z);  CE();
            f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

            m_shaders.m_k_means_clusterer.m_closest_cluster_finder.release();
        }

//        cluster_memberships.syncFromGPU();
//        int member_count[clusters.clusterCount()];
//        for(int i = 0; i < clusters.clusterCount(); i++)
//            member_count[i] = 0;
//        for(int x(0); x < cluster_memberships.width(); x++)
//        {
//            for(int y(0); y < cluster_memberships.height(); y++)
//            {
//                member_count[cluster_memberships(x,y)]++;
//            }
//        }
//        qCritical() << "************MEMBERSHIPS***************";
//        for(int i = 0; i < clusters.clusterCount(); i++)
//            qCritical() << i << " --> " << member_count[i];

        //        /*********************
//         * TMP *
//         *********************/
//        m_slope_and_humidity_cluster_reduction.syncFromGPU();
//        m_temperature_cluster_reduction.syncFromGPU();
//        m_daily_illumination_cluster_reduction.syncFromGPU();
//        qCritical() << "** WORK GROUP AVERAGES ***";
//        qCritical() << "Group count: [" << closest_cluster_group_count.x << "," << closest_cluster_group_count.y << "," << closest_cluster_group_count.z << "]";
//        float slope_avg(0);
//        float temp_avg(0);
//        float illumination_avg[2] = {0 , 0};
//        float humidity_avg[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
//        float total_member_count(0);
//        for(int x(0); x < closest_cluster_group_count.x; x++)
//        {
//            for(int y(0); y < closest_cluster_group_count.y; y++)
//            {
////                qCritical() << "---[" << x << "," << y << "]---";
//                // SLOPE & HUMIDITY
//                {
//                    uint index_x = x * 13;
//                    float slope = m_slope_and_humidity_cluster_reduction(0, index_x,y);
//                    index_x++;
//                    for(int i = 0; i < 12; i++)
//                    {
//                        humidity_avg[i] += m_slope_and_humidity_cluster_reduction(0, index_x+i,y);
//                    }
//                    //  Slope first
////                    qCritical() << "Slope avg: " << slope;
//                    slope_avg += slope;
//                }
//                // TEMPERATURE
//                {
//                    uint index_x = x*2;
//                    float member_count = m_temperature_cluster_reduction(0, index_x,y);
//                    float temp = m_temperature_cluster_reduction(0, index_x+1,y);

//                    total_member_count += member_count;
//                    temp_avg += temp;
////                    qCritical() << "Member count: " << m_temperature_cluster_reduction(0, index_x,y);
////                    qCritical() << "Avg temp: " << m_temperature_cluster_reduction(0, index_x+1,y);
//                }
//                // ILLUMINATION
//                {
//                    uint index_x = x*2;
//                    float min_i = m_daily_illumination_cluster_reduction(0, index_x,y);
//                    float max_i = m_daily_illumination_cluster_reduction(0, index_x+1,y);

//                    illumination_avg[0] += min_i;
//                    illumination_avg[1] += max_i;

////                    qCritical() << "Min illumination: " << min_i;
////                    qCritical() << "Max illumination: " << max_i;
//                }
//            }
//        }
//        qCritical() << "*** TOTAL AVERAGES ***";
//        slope_avg /= (closest_cluster_group_count.x*closest_cluster_group_count.y);
//        temp_avg /= (closest_cluster_group_count.x*closest_cluster_group_count.y);
//        for(int i = 0; i < 2; i++)
//        {
//            illumination_avg[i] /= (closest_cluster_group_count.x*closest_cluster_group_count.y);
//            illumination_avg[i] /= (closest_cluster_group_count.x*closest_cluster_group_count.y);
//        }
//        for(int i = 0; i < 12; i++)
//        {
//            humidity_avg[i] /= (closest_cluster_group_count.x*closest_cluster_group_count.y);
//        }

//        qCritical() << "Slope: " << slope_avg;
//        qCritical() << "Temp: " << temp_avg;
//        qCritical() << "Min illumination: " << illumination_avg[0];
//        qCritical() << "Max illumination: " << illumination_avg[1];
//        for(int i = 0; i < 12; i++)
//        {
//            qCritical() << "Humidity [" << i << "] --> " << humidity_avg[i];
//        }
//        qCritical() << "Total member count: " << total_member_count;
//        qCritical() << "*********************";

        /*********************
         * RECALCULATE MEANS *
         *********************/
        {
            m_shaders.m_k_means_clusterer.m_means_recalculator.bind();

            m_shaders.m_k_means_clusterer.m_means_recalculator.setUniformValue(Uniforms::Clustering::_N_CLUSTERS, clusters.clusterCount());

            /*****************************
             * SET CLUSTER DATA TEXTURES *
             *****************************/
            f->glBindImageTexture(0, clusters.m_slope_cluster_data.textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);  CE();
            // TEMPERATURE
            f->glBindImageTexture(1, clusters.m_temperature_cluster_data[5]->textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R8_SNORM);  CE();
            // DAILY ILLUMINATION
            f->glBindImageTexture(2, clusters.m_daily_illumination_cluster_data.textureId(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_R8);  CE();
            // WEIGHTED SOIL HUMIDITY
            f->glBindImageTexture(3, clusters.m_weighted_soil_humidity_cluster_data.textureId(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32F);  CE();

            /**********************************
             * SET CLUSTER REDUCTION TEXTURES *
             **********************************/
            // SLOPE AND SOIL HUMUDITY
            f->glBindImageTexture(4, m_slope_and_humidity_cluster_reduction.textureId(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32F);  CE();
            f->glBindImageTexture(5, m_temperature_cluster_reduction.textureId(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32F);  CE();
            f->glBindImageTexture(6, m_daily_illumination_cluster_reduction.textureId(), 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32F);  CE();

            // COMPUTE
            f->glDispatchCompute(means_recalculator_group_count.x, means_recalculator_group_count.y, means_recalculator_group_count.z);  CE();
            f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

            m_shaders.m_k_means_clusterer.m_means_recalculator.release();
        }
    }
}

glm::uvec3 Computer::calculateGroupCount(int n_threads_x, int n_threads_y, int n_threads_z,
                                         int group_size_x, int group_size_y, int group_size_z)
{
    GLuint n_groups_x(std::max(1, (int) std::ceil(((float)n_threads_x) / group_size_x)));
    GLuint n_groups_y(std::max(1, (int) std::ceil(((float)n_threads_y)/ group_size_y)));
    GLuint n_groups_z(std::max(1, (int) std::ceil(((float)n_threads_z)/ group_size_z)));

    return glm::uvec3(n_groups_x, n_groups_y, n_groups_z);
}
