#include "renderer.h"
#include "glheader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>

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
                             ResourceWrapper & resources,
                             const Transform & transforms,
                             const LightProperties & sunlight_properties,
                             const char * overlay,
                             int month)
{
    int texture_unit(GL_TEXTURE0); // TEXTURE_0 reserved for the heightmap
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    m_shaders.m_terrain.bind();

    m_shaders.m_terrain.setUniformValue(Uniforms::Transform::_PROJECTION, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._Proj)))); CE();
    m_shaders.m_terrain.setUniformValue(Uniforms::Transform::_VIEW, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._MV)))); CE();
    m_shaders.m_terrain.setUniformValue(Uniforms::Transform::_SCALE, transforms._scale); CE();

    // Month
    m_shaders.m_terrain.setUniformValue(Uniforms::Timing::_MONTH, month); CE();

    // Terrain heightmap texture
    {
        f->glActiveTexture(texture_unit); CE();
        m_shaders.m_terrain.setUniformValue(Uniforms::Texture::_TERRAIN_HEIGHTMAP, texture_unit-GL_TEXTURE0); CE();
        terrain.getDrawableTerrain().bind();CE();
        texture_unit++;
    }
    // Water heightmap texture
    {
        // Water heightmap
        f->glActiveTexture(texture_unit); CE();
        m_shaders.m_terrain.setUniformValue(Uniforms::Texture::_WATER_HEIGHTMAP, texture_unit-GL_TEXTURE0); CE();
        resources.getTerrainWater()[month].bind();CE();
        texture_unit++;
    }

    // Normals texture
    {
        f->glActiveTexture(texture_unit);CE();
        m_shaders.m_terrain.setUniformValue(Uniforms::Texture::_NORMALS, texture_unit-GL_TEXTURE0); CE();
        f->glBindTexture(GL_TEXTURE_2D, terrain.getNormals().getTextureUnit());CE();
        texture_unit++;
    }

    reset_overlays(m_shaders.m_terrain);
    m_shaders.m_terrain.setUniformValue(overlay, true); CE();

    // Bind shade texture
    if(overlay == Uniforms::Overlay::_SHADE)
    {
        f->glActiveTexture(texture_unit);CE();
        m_shaders.m_terrain.setUniformValue(Uniforms::Texture::_SHADE, texture_unit-GL_TEXTURE0); CE();
        resources.bindShade(); CE();
        texture_unit++;
    }
    else if(overlay == Uniforms::Overlay::_TEMPERATURE)
    {
        // June temperature
        {
            f->glActiveTexture(texture_unit);CE();
            m_shaders.m_terrain.setUniformValue(Uniforms::Texture::_TEMPERATURE_JUN, texture_unit-GL_TEXTURE0); CE();
            resources.bindJunTemperature(); CE();
            texture_unit++;
        }
        {
            f->glActiveTexture(texture_unit);CE();
            m_shaders.m_terrain.setUniformValue(Uniforms::Texture::_TEMPERATURE_DEC, texture_unit-GL_TEXTURE0); CE();
            resources.bindDecTemperature(); CE();
            texture_unit++;
        }
    }
    else if(overlay == Uniforms::Overlay::_MIN_DAILY_ILLUMINATION)
    {
        f->glActiveTexture(texture_unit);CE();
        m_shaders.m_terrain.setUniformValue(Uniforms::Texture::_MIN_DAILY_ILLUMINATION, texture_unit-GL_TEXTURE0); CE();
        resources.bindMinIllumination();
        texture_unit++;
    }
    else if(overlay == Uniforms::Overlay::_MAX_DAILY_ILLUMINATION)
    {
        f->glActiveTexture(texture_unit);CE();
        m_shaders.m_terrain.setUniformValue(Uniforms::Texture::_MAX_DAILY_ILLUMINATION, texture_unit-GL_TEXTURE0); CE();
        resources.bindMaxIllumination();
        texture_unit++;
    }

    // Heightmap maximum height
    m_shaders.m_terrain.setUniformValue(Uniforms::Terrain::_MAX_HEIGHT, (GLfloat)(terrain.getMaxHeight())); CE();

    // Heightmap base height
    m_shaders.m_terrain.setUniformValue(Uniforms::Terrain::_BASE_HEIGHT, (GLfloat)(terrain.getBaseHeight())); CE();

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
                            bool one_step)
{
    if(!terrain_water->isBalancing()) // Already balancing
    {
        terrain_water->setBalancing(true);

        int terrain_width (padded_terrain.width()-2);
        int terrain_depth (padded_terrain.height()-2);

        QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
        if(!f)
            qCritical() << "Could not obtain required OpenGL context version";
        f->initializeOpenGLFunctions();

        // Calculate the number of groups necessary
        GLuint n_groups_x(std::ceil(((float)terrain_width) / WaterFluxGeneratorShader::_GROUP_SIZE_X));
        GLuint n_groups_y(std::ceil(((float)terrain_depth)/ WaterFluxGeneratorShader::_GROUP_SIZE_Y));
        GLuint n_groups_z(1);

        // First set the cache terrain water to the current data
        {
            int sz(sizeof(GLuint) * terrain_width * terrain_depth);
            GLuint * cached_terrain_water = (GLuint*) std::malloc(sz);
            std::memcpy(cached_terrain_water, terrain_water->getRawData(), sz);

            m_terrain_water_cache.setData(cached_terrain_water, terrain_width, terrain_depth);
            m_terrain_water_cache.pushToGPU();
        }

        // Now set the counter texture data
        {
            int sz(sizeof(GLuint) * n_groups_x * n_groups_y);
            GLuint * counter_data = (GLuint*) malloc(sz);
            std::memset(counter_data, 0, sz);
            m_water_comparator_counter.setData(counter_data, n_groups_x, n_groups_y);

            m_water_comparator_counter.pushToGPU();
        }

        m_shaders.m_water_flux_generator.bind();  CE();

        /*******************
         * WATER HEIGHTMAP *
         *******************/
        f->glBindImageTexture(0, terrain_water->textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);  CE();
    //    m_shaders.m_water_flux_generator.setUniformValue(Uniforms::Texture::_WATER_HEIGHTMAP_JUN, 0);CE();

        /*********************
         * TERRAIN HEIGHTMAP *
         *********************/
        f->glBindImageTexture(1, padded_terrain.textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);  CE();
    //    m_shaders.m_water_flux_generator.setUniformValue(Uniforms::Texture::_TERRAIN_HEIGHTMAP, 1);CE();

        f->glDispatchCompute(n_groups_x, n_groups_y, n_groups_z);  CE();
        f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

        m_shaders.m_water_flux_generator.release();

        // Check for balance
        m_shaders.m_water_comparator.bind();
        f->glBindImageTexture(0, m_terrain_water_cache.textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);  CE();
        f->glBindImageTexture(1, terrain_water->textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);  CE();
        f->glBindImageTexture(2, m_water_comparator_counter.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);  CE();

        f->glDispatchCompute(n_groups_x, n_groups_y, n_groups_z);  CE();
        f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

        m_shaders.m_water_comparator.release();

        m_water_comparator_counter.syncFromGPU();
        GLuint agregated_changed_verticies(0);
        for(int x = 0; x < n_groups_x; x++)
        {
            for(int y = 0; y < n_groups_y; y++)
            {
                agregated_changed_verticies += m_water_comparator_counter(x, y);
            }
        }

        if(!one_step)
            terrain_water->incrementBalancingIteration(agregated_changed_verticies);

        terrain_water->setBalancing(false);
    }
}

void Renderer::reset_overlays(TerrainShader & shader)
{
    shader.setUniformValue(Uniforms::Overlay::_NONE, false); CE();
    shader.setUniformValue(Uniforms::Overlay::_SLOPE, false); CE();
    shader.setUniformValue(Uniforms::Overlay::_ALTITUDE, false); CE();
    shader.setUniformValue(Uniforms::Overlay::_SHADE, false); CE();
    shader.setUniformValue(Uniforms::Overlay::_TEMPERATURE, false); CE();
    shader.setUniformValue(Uniforms::Overlay::_MIN_DAILY_ILLUMINATION, false); CE();
    shader.setUniformValue(Uniforms::Overlay::_MAX_DAILY_ILLUMINATION, false); CE();
}

