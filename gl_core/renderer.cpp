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
                             GLuint water_heightmap_texture_id,
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
    {
        // Water heightmap
        f->glActiveTexture(texture_unit); CE();
        m_shaders.m_terrain.setUniformValue(Uniforms::Texture::_WATER_HEIGHTMAP, texture_unit-GL_TEXTURE0); CE();
        f->glBindTexture(GL_TEXTURE_2D, water_heightmap_texture_id);CE();
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
        m_shaders.m_terrain.setUniformValue(Uniforms::Texture::_OVERLAY, texture_unit-GL_TEXTURE0); CE();
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


