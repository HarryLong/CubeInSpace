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

void Renderer::calculateNormals(NormalsGeneratorShader & shader,
                                Terrain & terrain)
{
    int texture_unit(GL_TEXTURE0); // TEXTURE_0 reserved for the heightmap

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    shader.bind(); CE();

    // Save the viewport
    GLint viewport[4];
    f->glGetIntegerv(GL_VIEWPORT, viewport); // save current viewport


    f->glViewport(0,0,terrain.getWidth(), terrain.getDepth()); CE();

    // The heightmap texture
    {
        f->glActiveTexture(texture_unit); CE();
        shader.setUniformValue(Uniforms::Texture::_TERRAIN_HEIGHTMAP, texture_unit-GL_TEXTURE0); CE();    // Terrain size
        terrain.getDrawableTerrain().bind();
        texture_unit++;
    }

    // Calculate normals
    terrain.calculateNormals(); CE();

    // reset viewport
    f->glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);

    shader.release(); CE();
}

void Renderer::renderTerrain(TerrainShader & shader,
                             Terrain & terrain,
                             ResourceWrapper & resources,
                             const Transform & transforms,
                             const LightProperties & sunlight_properties,
                             const char * overlay,
                             int month)
{
    int texture_unit(GL_TEXTURE0); // TEXTURE_0 reserved for the heightmap
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    shader.bind();

    shader.setUniformValue(Uniforms::Transform::_PROJECTION, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._Proj)))); CE();
    shader.setUniformValue(Uniforms::Transform::_VIEW, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._MV)))); CE();
    shader.setUniformValue(Uniforms::Transform::_SCALE, transforms._scale); CE();

    // Month
    shader.setUniformValue(Uniforms::Timing::_MONTH, month); CE();

    // Terrain heightmap texture
    {
        f->glActiveTexture(texture_unit); CE();
        shader.setUniformValue(Uniforms::Texture::_TERRAIN_HEIGHTMAP, texture_unit-GL_TEXTURE0); CE();
        terrain.getDrawableTerrain().bind();CE();
        texture_unit++;
    }
    // Water heightmap texture
    {
        // June water heightmap
        {
            f->glActiveTexture(texture_unit); CE();
            shader.setUniformValue(Uniforms::Texture::_WATER_HEIGHTMAP_JUN, texture_unit-GL_TEXTURE0); CE();
            resources.bindJunWater(); CE();
            texture_unit++;
        }
        // Dec water heightmap
        {
            f->glActiveTexture(texture_unit); CE();
            shader.setUniformValue(Uniforms::Texture::_WATER_HEIGHTMAP_DEC, texture_unit-GL_TEXTURE0); CE();
            resources.bindDecWater(); CE();
            texture_unit++;
        }
    }

    // Normals texture
    {
        f->glActiveTexture(texture_unit);CE();
        shader.setUniformValue(Uniforms::Texture::_NORMALS, texture_unit-GL_TEXTURE0); CE();
        f->glBindTexture(GL_TEXTURE_2D, terrain.getNormals().getTextureUnit());CE();
        texture_unit++;
    }

    reset_overlays(shader);
    shader.setUniformValue(overlay, true); CE();

    // Bind shade texture
    if(overlay == Uniforms::Overlay::_SHADE)
    {
        f->glActiveTexture(texture_unit);CE();
        shader.setUniformValue(Uniforms::Texture::_SHADE, texture_unit-GL_TEXTURE0); CE();
        resources.bindShade(); CE();
        texture_unit++;
    }
    else if(overlay == Uniforms::Overlay::_TEMPERATURE)
    {
        // June temperature
        {
            f->glActiveTexture(texture_unit);CE();
            shader.setUniformValue(Uniforms::Texture::_TEMPERATURE_JUN, texture_unit-GL_TEXTURE0); CE();
            resources.bindJunTemperature(); CE();
            texture_unit++;
        }
        {
            f->glActiveTexture(texture_unit);CE();
            shader.setUniformValue(Uniforms::Texture::_TEMPERATURE_DEC, texture_unit-GL_TEXTURE0); CE();
            resources.bindDecTemperature(); CE();
            texture_unit++;
        }
    }
    else if(overlay == Uniforms::Overlay::_MIN_DAILY_ILLUMINATION)
    {
        f->glActiveTexture(texture_unit);CE();
        shader.setUniformValue(Uniforms::Texture::_MIN_DAILY_ILLUMINATION, texture_unit-GL_TEXTURE0); CE();
        resources.bindMinIllumination();
        texture_unit++;
    }
    else if(overlay == Uniforms::Overlay::_MAX_DAILY_ILLUMINATION)
    {
        f->glActiveTexture(texture_unit);CE();
        shader.setUniformValue(Uniforms::Texture::_MAX_DAILY_ILLUMINATION, texture_unit-GL_TEXTURE0); CE();
        resources.bindMaxIllumination();
        texture_unit++;
    }

    // Heightmap maximum height
    shader.setUniformValue(Uniforms::Terrain::_MAX_HEIGHT, (GLfloat)(terrain.getMaxHeight())); CE();

    // Heightmap base height
    shader.setUniformValue(Uniforms::Terrain::_BASE_HEIGHT, (GLfloat)(terrain.getBaseHeight())); CE();

    // Terrain Material properties
    {
        shader.setUniformValueArray(Uniforms::Terrain::_MATERIAL_AMBIENT, glm::value_ptr(Terrain::MaterialProperties::_AMBIENT), 1, 4); CE();
        shader.setUniformValueArray(Uniforms::Terrain::_MATERIAL_DIFFUSE, glm::value_ptr(Terrain::MaterialProperties::_DIFFUSE), 1, 4); CE();
    }
    // Water Material properties
    {
        shader.setUniformValueArray(Uniforms::Water::_MATERIAL_AMBIENT, glm::value_ptr(TerrainWater::MaterialProperties::_AMBIENT), 1, 4); CE();
        shader.setUniformValueArray(Uniforms::Water::_MATERIAL_DIFFUSE, glm::value_ptr(TerrainWater::MaterialProperties::_DIFFUSE), 1, 4); CE();
    }

    // Sun Light
    shader.setUniformValueArray(Uniforms::Lighting::_DIFFUSE_COLOR, glm::value_ptr(sunlight_properties.m_diffuse_color), 1, 4); CE();
    shader.setUniformValueArray(Uniforms::Lighting::_AMBIENT_COLOR, glm::value_ptr(sunlight_properties.m_ambient_color), 1, 4); CE();
    shader.setUniformValueArray(Uniforms::Lighting::_POSITION, glm::value_ptr(sunlight_properties.getPosition()), 1, 3); CE();

    terrain.render();

    shader.release();
}

void Renderer::renderTerrainElements(TerrainElementsShader & shader,
                                     Terrain & terrain,
                                     Transform & transforms,
                                     const std::vector<Asset*> & p_assets)
{
    int texture_unit(GL_TEXTURE0); // TEXTURE_0 reserved for the heightmap
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    shader.bind();

    shader.setUniformValue(Uniforms::Transform::_PROJECTION, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._Proj)))); CE();
    shader.setUniformValue(Uniforms::Transform::_VIEW, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._MV)))); CE();

    // The heightmap texture
    {
        f->glActiveTexture(texture_unit); CE();
        shader.setUniformValue(Uniforms::Texture::_TERRAIN_HEIGHTMAP, texture_unit-GL_TEXTURE0); CE();
        terrain.getDrawableTerrain().bind();
        texture_unit++;
    }

    // Draw all the assets
    for(Asset * asset : p_assets)
    {
        shader.setUniformValue(Uniforms::Assets::_USE_UNIFORM_COLOR, asset->m_uniform_color ); CE();

        if(asset->m_uniform_color) // Color data attached with verticies
        {
            glm::vec4 color(asset->m_color);
            shader.setUniformValueArray(Uniforms::Assets::_UNIFORM_COLOR, glm::value_ptr(color), 1, 4 ); CE();
        }

        for(Asset::AssetTransformations asset_transform : asset->getTransformations())
        {
            shader.setUniformValue(Uniforms::Transform::_MODEL, QMatrix4x4(glm::value_ptr(glm::transpose(asset_transform.mtw)))); CE();
            shader.setUniformValue(Uniforms::Transform::_SCALE, asset_transform.scale); CE();

            asset->render();
        }
    }

    shader.release();
}

void Renderer::renderAssets(BaseShader & shader,
                            const Transform & transforms,
                            const std::vector<Asset*> & p_assets)
{
    shader.bind();

    shader.setUniformValue(Uniforms::Transform::_PROJECTION, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._Proj)))); CE();
    shader.setUniformValue(Uniforms::Transform::_VIEW, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._MV)))); CE();;

    // Draw all the assets
    for(Asset * asset : p_assets)
    {
        shader.setUniformValue(Uniforms::Assets::_USE_UNIFORM_COLOR, asset->m_uniform_color ); CE();

        if(asset->m_uniform_color) // Color data attached with verticies
        {
            glm::vec4 color(asset->m_color);
            shader.setUniformValueArray(Uniforms::Assets::_UNIFORM_COLOR, glm::value_ptr(color), 1, 4 ); CE();
        }

        for(Asset::AssetTransformations asset_transform : asset->getTransformations())
        {
            shader.setUniformValue(Uniforms::Transform::_MODEL, QMatrix4x4(glm::value_ptr(glm::transpose(asset_transform.mtw)))); CE();
            shader.setUniformValue(Uniforms::Transform::_SCALE, asset_transform.scale); CE();

            asset->render();
        }
    }

    shader.release();
}

void Renderer::balanceWater(WaterFluxGeneratorShader shader,
                            Terrain & terrain,
                            GLuint terrain_water_heightmap_texture_id)
{
    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    shader.bind();  CE();

    /*******************
     * WATER HEIGHTMAP *
     *******************/
    f->glBindImageTexture(0, terrain_water_heightmap_texture_id, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);  CE();
    shader.setUniformValue(Uniforms::Texture::_WATER_HEIGHTMAP_JUN, 0);CE();

    /*********************
     * TERRAIN HEIGHTMAP *
     *********************/
    f->glBindImageTexture(1, terrain.getDrawableTerrain().textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);  CE();
    shader.setUniformValue(Uniforms::Texture::_TERRAIN_HEIGHTMAP, 1);CE();

    // Calculate the number of groups necessary
    GLuint n_groups_x(std::ceil(((float)terrain.getWidth()) / WaterFluxGeneratorShader::_GROUP_SIZE_X));
    GLuint n_groups_y(std::ceil(((float)terrain.getDepth())/ WaterFluxGeneratorShader::_GROUP_SIZE_Y));
    GLuint n_groups_z(1);

    /*************************
     * COUNTER IMAGE TEXTURE *
     *************************/
    int sz(sizeof(GLuint) * n_groups_x * n_groups_y);
    GLuint * counter_data = (GLuint*) malloc(sz);
    memset(counter_data, 0, sz);
    m_counter_texture.setData(counter_data, n_groups_x, n_groups_y);
    m_counter_texture.pushToGPU();

    f->glBindImageTexture(2, m_counter_texture.textureId(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);  CE();
    shader.setUniformValue(Uniforms::Counter::_VERTICIES_EMPTIED, 2);CE();

    f->glDispatchCompute(n_groups_x, n_groups_y, n_groups_z);  CE();
    f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();

    shader.release();

    m_counter_texture.syncFromGPU();
    GLuint agregated_emptied_verticies(0);
    for(int x = 0; x < n_groups_x; x++)
    {
        for(int y = 0; y < n_groups_y; y++)
        {
            agregated_emptied_verticies += m_counter_texture(x, y);
        }
    }

    qCritical() << "Emptied verticies: " << agregated_emptied_verticies;
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

