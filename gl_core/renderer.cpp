#include "renderer.h"
#include "glheader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include <algorithm>

Renderer::Renderer()
{
}

Renderer::~Renderer()
{

}

void Renderer::compileAndLinkShaders()
{
    m_shaders.compileAndLink();
}

void Renderer::calculateNormals(Terrain & terrain)
{
    int texture_unit(GL_TEXTURE0); // TEXTURE_0 reserved for the heightmap

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    m_shaders.m_normals_generator.bind(); CE();

    // Save the viewport
    GLint viewport[4];
    f->glGetIntegerv(GL_VIEWPORT, viewport); // save current viewport


    f->glViewport(0,0,terrain.getWidth(), terrain.getDepth()); CE();

    // The heightmap texture
    {
        f->glActiveTexture(texture_unit); CE();
        m_shaders.m_normals_generator.setUniformValue(Uniforms::Texture::_TERRAIN_HEIGHTMAP, texture_unit-GL_TEXTURE0); CE();    // Terrain size
        terrain.getDrawableTerrain().bind();
        texture_unit++;
    }

    // Calculate normals
    terrain.calculateNormals(); CE();

    // reset viewport
    f->glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);

    m_shaders.m_normals_generator.release(); CE();
}

void Renderer::renderTerrain(Terrain & terrain,
                             TerrainWaterHeightmap & water_heightmap,
                             bool render_water,
                             const Transform & transforms,
                             const LightProperties & sunlight_properties,
                             GLuint overlay_texture_id,
                             bool overlay_active)
{    
    int texture_unit(GL_TEXTURE0); // TEXTURE_0 reserved for the heightmap
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    m_shaders.m_terrain.bind();

    m_shaders.m_terrain.setUniformValue(Uniforms::Transform::_PROJECTION, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._Proj)))); CE();
    m_shaders.m_terrain.setUniformValue(Uniforms::Transform::_VIEW, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._MV)))); CE();
    m_shaders.m_terrain.setUniformValue(Uniforms::Transform::_SCALE, transforms._scale); CE();
    m_shaders.m_terrain.setUniformValue(Uniforms::Terrain::_SCALE, terrain.getScale()); CE();
    m_shaders.m_terrain.setUniformValue(Uniforms::Water::_RENDER, render_water); CE();

    // Terrain heightmap texture
    {
        f->glActiveTexture(texture_unit); CE();
        m_shaders.m_terrain.setUniformValue(Uniforms::Texture::_TERRAIN_HEIGHTMAP, texture_unit-GL_TEXTURE0); CE();
        terrain.getDrawableTerrain().bind();CE();
        texture_unit++;
    }
    // Water heightmap texture
    if(render_water)
    {
        // Water heightmap
        f->glActiveTexture(texture_unit); CE();
        m_shaders.m_terrain.setUniformValue(Uniforms::Texture::_WATER_HEIGHTMAP, texture_unit-GL_TEXTURE0); CE();
        water_heightmap.bind();CE();
        texture_unit++;
    }

    // Normals texture
    {
        f->glActiveTexture(texture_unit);CE();
        m_shaders.m_terrain.setUniformValue(Uniforms::Texture::_NORMALS, texture_unit-GL_TEXTURE0); CE();
        f->glBindTexture(GL_TEXTURE_2D, terrain.getNormals().getTextureUnit());CE();
        texture_unit++;
    }

    // Overlay texture
    m_shaders.m_terrain.setUniformValue(Uniforms::Overlay::_ACTIVE, overlay_active);
    if(overlay_active)
    {
        f->glActiveTexture(texture_unit);CE();
        m_shaders.m_terrain.setUniformValue(Uniforms::Overlay::_TEXTURE, texture_unit-GL_TEXTURE0); CE();
        f->glBindTexture(GL_TEXTURE_2D, overlay_texture_id);CE();
        texture_unit++;
    }

    // Terrain Material properties
    {
        m_shaders.m_terrain.setUniformValueArray(Uniforms::Terrain::_MATERIAL_AMBIENT, glm::value_ptr(Terrain::MaterialProperties::_AMBIENT), 1, 4); CE();
        m_shaders.m_terrain.setUniformValueArray(Uniforms::Terrain::_MATERIAL_DIFFUSE, glm::value_ptr(Terrain::MaterialProperties::_DIFFUSE), 1, 4); CE();
    }
    // Water Material properties
    {
        m_shaders.m_terrain.setUniformValueArray(Uniforms::Water::_MATERIAL_AMBIENT, glm::value_ptr(TerrainWater::MaterialProperties::_AMBIENT), 1, 4); CE();
        m_shaders.m_terrain.setUniformValueArray(Uniforms::Water::_MATERIAL_DIFFUSE, glm::value_ptr(TerrainWater::MaterialProperties::_DIFFUSE), 1, 4); CE();
    }

    // Sun Light
    m_shaders.m_terrain.setUniformValueArray(Uniforms::Lighting::_DIFFUSE_COLOR, glm::value_ptr(sunlight_properties.m_diffuse_color), 1, 4); CE();
    m_shaders.m_terrain.setUniformValueArray(Uniforms::Lighting::_AMBIENT_COLOR, glm::value_ptr(sunlight_properties.m_ambient_color), 1, 4); CE();
    m_shaders.m_terrain.setUniformValueArray(Uniforms::Lighting::_POSITION, glm::value_ptr(sunlight_properties.getPosition()), 1, 3); CE();

    terrain.render();

    m_shaders.m_terrain.release();
}

void Renderer::createOverlayTexture(GLuint overlay_texture_id, Terrain & terrain, ResourceWrapper & resources, const char * active_overlay, int month)
{
    int texture_unit(GL_TEXTURE0); // TEXTURE_0 reserved for the heightmap
    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    m_shaders.m_overlay_texture_creator.bind();
    reset_overlays(m_shaders.m_overlay_texture_creator);
    m_shaders.m_overlay_texture_creator.setUniformValue(active_overlay, true); CE();

    // Month
    m_shaders.m_overlay_texture_creator.setUniformValue(Uniforms::Timing::_MONTH, month); CE();

    if(active_overlay == Uniforms::Overlay::_ALTITUDE)
    {
        // Heightmap min height
        m_shaders.m_overlay_texture_creator.setUniformValue(Uniforms::Terrain::_MIN_HEIGHT, (GLfloat)(terrain.getMinHeight())); CE();
        // Heightmap max height
        m_shaders.m_overlay_texture_creator.setUniformValue(Uniforms::Terrain::_MAX_HEIGHT, (GLfloat)(terrain.getMaxHeight())); CE();

        f->glActiveTexture(texture_unit); CE();
        m_shaders.m_overlay_texture_creator.setUniformValue(Uniforms::Texture::_TERRAIN_HEIGHTMAP, texture_unit-GL_TEXTURE0); CE();
        terrain.getDrawableTerrain().bind();CE();
        texture_unit++;
    }
    else if(active_overlay == Uniforms::Overlay::_SLOPE)
    {
        f->glActiveTexture(texture_unit);CE();
        m_shaders.m_overlay_texture_creator.setUniformValue(Uniforms::Texture::_NORMALS, texture_unit-GL_TEXTURE0); CE();
        f->glBindTexture(GL_TEXTURE_2D, terrain.getNormals().getTextureUnit());CE();
        texture_unit++;
    }
    else if(active_overlay == Uniforms::Overlay::_SHADE)
    {
        f->glActiveTexture(texture_unit);CE();
        m_shaders.m_overlay_texture_creator.setUniformValue(Uniforms::Texture::_SHADE, texture_unit-GL_TEXTURE0); CE();
        resources.getShade().bind(); CE();
        texture_unit++;
    }
    else if(active_overlay == Uniforms::Overlay::_TEMPERATURE)
    {
        TerrainTemperature & temp (resources.getTerrainTemp());
        // June temperature
        {
            f->glActiveTexture(texture_unit);CE();
            m_shaders.m_overlay_texture_creator.setUniformValue(Uniforms::Texture::_TEMPERATURE_JUN, texture_unit-GL_TEXTURE0); CE();
            temp.bindJun(); CE();
            texture_unit++;
        }
        // December Temp
        {
            f->glActiveTexture(texture_unit);CE();
            m_shaders.m_overlay_texture_creator.setUniformValue(Uniforms::Texture::_TEMPERATURE_DEC, texture_unit-GL_TEXTURE0); CE();
            temp.bindDec(); CE();
            texture_unit++;
        }
    }
    else if(active_overlay == Uniforms::Overlay::_MIN_DAILY_ILLUMINATION)
    {
        f->glActiveTexture(texture_unit);CE();
        m_shaders.m_overlay_texture_creator.setUniformValue(Uniforms::Texture::_MIN_DAILY_ILLUMINATION, texture_unit-GL_TEXTURE0); CE();
        resources.getDailyIllumination().bindMin();CE();
        texture_unit++;
    }
    else if(active_overlay == Uniforms::Overlay::_MAX_DAILY_ILLUMINATION)
    {
        f->glActiveTexture(texture_unit);CE();
        m_shaders.m_overlay_texture_creator.setUniformValue(Uniforms::Texture::_MAX_DAILY_ILLUMINATION, texture_unit-GL_TEXTURE0); CE();
        resources.getDailyIllumination().bindMax();CE();
        texture_unit++;
    }
    else if(active_overlay == Uniforms::Overlay::_SOIL_INFILTRATION_RATE)
    {
        f->glActiveTexture(texture_unit);CE();
        m_shaders.m_overlay_texture_creator.setUniformValue(Uniforms::Texture::_SOIL_INFILTRATION_RATE, texture_unit-GL_TEXTURE0); CE();
        resources.getSoilInfiltration().bind();
        texture_unit++;
    }
    else if(active_overlay == Uniforms::Overlay::_MONTHLY_SOIL_HUMIDITY)
    {
        f->glActiveTexture(texture_unit);CE();
        m_shaders.m_overlay_texture_creator.setUniformValue(Uniforms::Texture::_MONTHLY_SOIL_HUMIDITY, texture_unit-GL_TEXTURE0); CE();
        resources.getSoilHumidity()[month].bind();
        texture_unit++;
    }
    else if(active_overlay == Uniforms::Overlay::_WEIGHTED_AVG_SOIL_HUMIDITY)
    {
        f->glActiveTexture(texture_unit);CE();
        m_shaders.m_overlay_texture_creator.setUniformValue(Uniforms::Texture::_WEIGHTED_AVG_SOIL_HUMIDITY, texture_unit-GL_TEXTURE0); CE();
        resources.getWeightedSoilHumidity()[month].bind();
        texture_unit++;
    }

    glm::vec3 group_count ( calculateGroupCount(terrain.getWidth(), terrain.getDepth(), 1,
                                        OverlayTextureCreatorShader::_GROUP_SIZE_X,
                                        OverlayTextureCreatorShader::_GROUP_SIZE_Y,
                                        OverlayTextureCreatorShader::_GROUP_SIZE_Z));

    f->glBindImageTexture(0, overlay_texture_id, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);  CE();

    f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
    f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();


    m_shaders.m_overlay_texture_creator.release();
}

void Renderer::renderTerrainElements(Terrain & terrain,
                                     Transform & transforms,
                                     const std::vector<Asset*> & p_assets)
{
    int texture_unit(GL_TEXTURE0); // TEXTURE_0 reserved for the heightmap
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    m_shaders.m_terrain_elements.bind();

    m_shaders.m_terrain_elements.setUniformValue(Uniforms::Transform::_PROJECTION, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._Proj)))); CE();
    m_shaders.m_terrain_elements.setUniformValue(Uniforms::Transform::_VIEW, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._MV)))); CE();

    // The heightmap texture
    {
        f->glActiveTexture(texture_unit); CE();
        m_shaders.m_terrain_elements.setUniformValue(Uniforms::Texture::_TERRAIN_HEIGHTMAP, texture_unit-GL_TEXTURE0); CE();
        terrain.getDrawableTerrain().bind();
        texture_unit++;
    }

    // Draw all the assets
    for(Asset * asset : p_assets)
    {
        m_shaders.m_terrain_elements.setUniformValue(Uniforms::Assets::_USE_UNIFORM_COLOR, asset->m_uniform_color ); CE();

        if(asset->m_uniform_color) // Color data attached with verticies
        {
            glm::vec4 color(asset->m_color);
            m_shaders.m_terrain_elements.setUniformValueArray(Uniforms::Assets::_UNIFORM_COLOR, glm::value_ptr(color), 1, 4 ); CE();
        }

        for(Asset::AssetTransformations asset_transform : asset->getTransformations())
        {
            m_shaders.m_terrain_elements.setUniformValue(Uniforms::Transform::_MODEL, QMatrix4x4(glm::value_ptr(glm::transpose(asset_transform.mtw)))); CE();
            m_shaders.m_terrain_elements.setUniformValue(Uniforms::Transform::_SCALE, asset_transform.scale); CE();

            asset->render();
        }
    }

    m_shaders.m_terrain_elements.release();
}

void Renderer::renderAssets(const Transform & transforms,
                            const std::vector<Asset*> & p_assets)
{
    m_shaders.m_base.bind();

    m_shaders.m_base.setUniformValue(Uniforms::Transform::_PROJECTION, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._Proj)))); CE();
    m_shaders.m_base.setUniformValue(Uniforms::Transform::_VIEW, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._MV)))); CE();;

    // Draw all the assets
    for(Asset * asset : p_assets)
    {
        m_shaders.m_base.setUniformValue(Uniforms::Assets::_USE_UNIFORM_COLOR, asset->m_uniform_color ); CE();

        if(asset->m_uniform_color) // Color data attached with verticies
        {
            glm::vec4 color(asset->m_color);
            m_shaders.m_base.setUniformValueArray(Uniforms::Assets::_UNIFORM_COLOR, glm::value_ptr(color), 1, 4 ); CE();
        }

        for(Asset::AssetTransformations asset_transform : asset->getTransformations())
        {
            m_shaders.m_base.setUniformValue(Uniforms::Transform::_MODEL, QMatrix4x4(glm::value_ptr(glm::transpose(asset_transform.mtw)))); CE();
            m_shaders.m_base.setUniformValue(Uniforms::Transform::_SCALE, asset_transform.scale); CE();

            asset->render();
        }
    }

    m_shaders.m_base.release();
}

void Renderer::balanceWater(PaddedTerrain & padded_terrain,
                            TerrainWaterHeightmap * terrain_water,
                            float terrain_scale,
                            bool one_step)
{
    if(!terrain_water->isBalancing()) // Already balancing
    {
        /*****************
         * BALANCE WATER *
         *****************/
        terrain_water->setBalancing(true);

        int terrain_width (padded_terrain.width()-2);
        int terrain_depth (padded_terrain.height()-2);

        QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
        if(!f)
            qCritical() << "Could not obtain required OpenGL context version";
        f->initializeOpenGLFunctions();

        glm::uvec3 group_count (calculateGroupCount(terrain_width, terrain_depth, 1,
                                                    WaterFluxGeneratorShader::_GROUP_SIZE_X,
                                                    WaterFluxGeneratorShader::_GROUP_SIZE_Y,
                                                    WaterFluxGeneratorShader::_GROUP_SIZE_Z));

        // Setup overlapping heightmaps
        {
            int width (terrain_water->width());
            int height (group_count.y*2);
            int sz(width*height*sizeof(GLfloat));
            GLfloat * data = (GLfloat*) std::malloc(sz);
            std::memset(data, 0, sz);
            m_horizontal_overlaps.setData(data, width, height);
        }
        {
            int width (group_count.x*2);
            int height (terrain_water->height());
            int sz(width*height*sizeof(GLfloat));
            GLfloat * data = (GLfloat*) std::malloc(sz);
            std::memset(data, 0, sz);
            m_vertical_overlaps.setData(data, width, height);
        }
        {
            int width (group_count.x*2);
            int height (group_count.y*2);
            int sz(width*height*sizeof(GLfloat));
            GLfloat * data = (GLfloat*) std::malloc(sz);
            std::memset(data, 0, sz);
            m_corner_overlaps.setData(data, width, height);
        }

        // First set the cache terrain water to the current data
        {
            int sz(sizeof(GLfloat) * terrain_width * terrain_depth);
            GLfloat * cached_terrain_water = (GLfloat*) std::malloc(sz);
            std::memcpy(cached_terrain_water, terrain_water->getRawData(), sz);
            m_terrain_water_cache.setData(cached_terrain_water, terrain_width, terrain_depth);
        }

        // Now set the counter texture data
        {
            int sz(sizeof(GLuint) * group_count.x * group_count.y * group_count.z);
            GLuint * counter_data = (GLuint*) malloc(sz);
            std::memset(counter_data, 0, sz);
            m_water_comparator_counter.setData(counter_data, group_count.x, group_count.y);
        }

        m_shaders.m_water_flux_generator.bind();  CE();

        f->glBindImageTexture(0, terrain_water->textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);  CE();
        f->glBindImageTexture(1, padded_terrain.textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);  CE();
        f->glBindImageTexture(2, m_horizontal_overlaps.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);  CE();
        f->glBindImageTexture(3, m_vertical_overlaps.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);  CE();
        f->glBindImageTexture(4, m_corner_overlaps.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);  CE();

        f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
        f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

//        //TMP
//        terrain_water->syncFromGPU();
//        qCritical() << "***********BEFORE BORDER CONTROL*******************";
//        for(int y = 0; y < terrain_water->height(); y++)
//        {
//            for(int x = 0; x < terrain_water->width(); x++)
//            {
//                qCritical() << "[" << x << ", " << y << "] --> " << (*terrain_water)(x,y);
//            }
//        }

        m_shaders.m_water_flux_generator.release();

//        m_horizontal_overlaps.syncFromGPU();
//        m_vertical_overlaps.syncFromGPU();
//        m_corner_overlaps.syncFromGPU();
//        qCritical() << "***********HORIZONTAL BORDERS*******************";
//        for(int y = 0; y < m_horizontal_overlaps.height(); y++)
//        {
//            for(int x = 0; x < m_horizontal_overlaps.width(); x++)
//            {
//                qCritical() << "[" << x << ", " << y << " --> " << m_horizontal_overlaps(x,y);
//            }
//        }
//        qCritical() << "***********VERTICAL BORDERS*******************";
//        for(int x = 0; x < m_vertical_overlaps.width(); x++)
//        {
//            for(int y = 0; y < m_vertical_overlaps.height(); y++)
//            {
//                qCritical() << "[" << x << ", " << y << " --> " << m_vertical_overlaps(x,y);
//            }
//        }
//        qCritical() << "***********CORNER BORDERS*******************";
//        for(int x = 0; x < m_corner_overlaps.width(); x++)
//        {
//            for(int y = 0; y < m_corner_overlaps.height(); y++)
//            {
//                qCritical() << "[" << x << ", " << y << " --> " << m_corner_overlaps(x,y);
//            }
//        }

        /********************
         * BORDER REDUCTION *
         ********************/
        m_shaders.m_border_overlap_reducer.bind();

        f->glBindImageTexture(0, terrain_water->textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);  CE();

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

        //TMP
//        terrain_water->syncFromGPU();
//        qCritical() << "***********POST BORDER CONTROL*******************";
//        for(int y = 0; y < terrain_water->height(); y++)
//        {
//            for(int x = 0; x < terrain_water->width(); x++)
//            {
//                qCritical() << "[" << x << ", " << y << "] --> " << (*terrain_water)(x,y);
//            }
//        }

        /*****************
         * BALANCE CHECK *
         *****************/
        group_count = calculateGroupCount(terrain_width, terrain_depth, 1,
                                            WaterComparatorShader::_GROUP_SIZE_X,
                                            WaterComparatorShader::_GROUP_SIZE_Y,
                                            WaterComparatorShader::_GROUP_SIZE_Z);
        m_shaders.m_water_comparator.bind();
        f->glBindImageTexture(0, m_terrain_water_cache.textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);  CE();
        f->glBindImageTexture(1, terrain_water->textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);  CE();
        f->glBindImageTexture(2, m_water_comparator_counter.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);  CE();

        f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
        f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

        m_shaders.m_water_comparator.release();

        m_water_comparator_counter.syncFromGPU();
        GLuint agregated_changed_verticies(0);
        for(int x = 0; x < group_count.x; x++)
        {
            for(int y = 0; y < group_count.y; y++)
            {
                agregated_changed_verticies += m_water_comparator_counter(x, y);
            }
        }

        if(!one_step)
            terrain_water->incrementBalancingIteration(agregated_changed_verticies);

        terrain_water->setBalancing(false);
    }
}

void Renderer::calculateSoilHumidityAndStandingWater(GLuint soil_infiltration_texture_id,
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

void Renderer::calculateWeightedSoilHumidity(SoilHumidity & soil_humdities,
                                             WeightedSoilHumidity & weighted_soil_humidities)
{
    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    glm::uvec3 group_count (calculateGroupCount(soil_humdities[1].width(), soil_humdities[1].height(), 1,
                                                WeightedAverageCalculator::_GROUP_SIZE_X,
                                                WeightedAverageCalculator::_GROUP_SIZE_Y,
                                                WeightedAverageCalculator::_GROUP_SIZE_Z));

    m_shaders.m_weighted_avg_calculator.bind();

    for(int i(0); i < 12; i++)
    {
        f->glBindImageTexture(0, soil_humdities[i+1].textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);  CE(); // Soil humidity
        f->glBindImageTexture(1, soil_humdities[((i+11) % 12)+1 ].textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);  CE(); // Soil humidity
        f->glBindImageTexture(2, soil_humdities[((i+10) % 12)+1].textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);  CE(); // Soil humidity

        f->glBindImageTexture(3, weighted_soil_humidities[i+1].textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);  CE(); // Resulting weighted soil humidity

        f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
        f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();
    }

    m_shaders.m_weighted_avg_calculator.release();
}

void Renderer::slopeBasedInfiltrationRateFilter(Terrain & terrain,
                                                    GLuint soil_infiltration_texture_id,
                                                    int min_slope)
{
    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    glm::uvec3 group_count (calculateGroupCount(terrain.getWidth(), terrain.getDepth(), 1,
                                                SlopeBasedSoilInfiltrationShader::_GROUP_SIZE_X,
                                                SlopeBasedSoilInfiltrationShader::_GROUP_SIZE_Y,
                                                SlopeBasedSoilInfiltrationShader::_GROUP_SIZE_Z));

    m_shaders.m_slope_based_soil_infiltration_calculator.bind();

    m_shaders.m_slope_based_soil_infiltration_calculator.setUniformValue(Uniforms::SlopeBasedSoilInfiltration::_MIN_SLOPE, min_slope);

    // Normals texture
    {
        f->glActiveTexture(0);CE();
        m_shaders.m_slope_based_soil_infiltration_calculator.setUniformValue(Uniforms::Texture::_NORMALS, 0); CE();
        f->glBindTexture(GL_TEXTURE_2D, terrain.getNormals().getTextureUnit());CE();
    }
    f->glBindImageTexture(1, soil_infiltration_texture_id, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);  CE();

    f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
    f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

     m_shaders.m_slope_based_soil_infiltration_calculator.release();
}


void Renderer::setAbsoluteAggregateHeight(Terrain & terrain,
                                TerrainWaterHeightmap & terrain_water,
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

    f->glBindImageTexture(0, terrain_water.textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);  CE();
    f->glBindImageTexture(1, terrain.getDrawableTerrain().textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);  CE();

    f->glDispatchCompute(group_count.x, group_count.y, group_count.z);  CE();
    f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

    m_shaders.m_aggregate_height.release();
}

glm::uvec3 Renderer::calculateGroupCount(int n_threads_x, int n_threads_y, int n_threads_z,
                                         int group_size_x, int group_size_y, int group_size_z)
{
    GLuint n_groups_x(std::max(1, (int) std::ceil(((float)n_threads_x) / group_size_x)));
    GLuint n_groups_y(std::max(1, (int) std::ceil(((float)n_threads_y)/ group_size_y)));
    GLuint n_groups_z(std::max(1, (int) std::ceil(((float)n_threads_z)/ group_size_z)));

    return glm::uvec3(n_groups_x, n_groups_y, n_groups_z);
}

void Renderer::reset_overlays(OverlayTextureCreatorShader & shader)
{
    shader.setUniformValue(Uniforms::Overlay::_SLOPE, false); CE();
    shader.setUniformValue(Uniforms::Overlay::_ALTITUDE, false); CE();
    shader.setUniformValue(Uniforms::Overlay::_SHADE, false); CE();
    shader.setUniformValue(Uniforms::Overlay::_TEMPERATURE, false); CE();
    shader.setUniformValue(Uniforms::Overlay::_MIN_DAILY_ILLUMINATION, false); CE();
    shader.setUniformValue(Uniforms::Overlay::_MAX_DAILY_ILLUMINATION, false); CE();
    shader.setUniformValue(Uniforms::Overlay::_SOIL_INFILTRATION_RATE, false); CE();
    shader.setUniformValue(Uniforms::Overlay::_MONTHLY_SOIL_HUMIDITY, false); CE();
    shader.setUniformValue(Uniforms::Overlay::_WEIGHTED_AVG_SOIL_HUMIDITY, false); CE();
}

