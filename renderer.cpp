#include "renderer.h"
#include "glheader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include "shader_programs.h"
#include "uniforms.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{

}

void Renderer::calculateNormals(QOpenGLShaderProgram * shader, Terrain * terrain)
{
    int texture_unit(GL_TEXTURE0); // TEXTURE_0 reserved for the heightmap

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    shader->bind(); CE();

    // Save the viewport
    GLint viewport[4];
    f->glGetIntegerv(GL_VIEWPORT, viewport); // save current viewport


    f->glViewport(0,0,terrain->getWidth(), terrain->getDepth()); CE();

    // The heightmap texture
    {
        f->glActiveTexture(texture_unit); CE();
        shader->setUniformValue(Uniforms::Texture::_TERRAIN_HEIGHTMAP, texture_unit-GL_TEXTURE0); CE();    // Terrain size
        terrain->getDrawableTerrain().bind();
        texture_unit++;
    }

    // Calculate normals
    terrain->renderNormals(); CE();

    // reset viewport
    f->glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);

    shader->release(); CE();
}

void Renderer::renderTerrain(QOpenGLShaderProgram * shader, const Transform & transforms, Terrain * terrain,
                             const LightProperties & sunlight_properties, int month)
{
    int texture_unit(GL_TEXTURE0); // TEXTURE_0 reserved for the heightmap
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    shader->bind();

    shader->setUniformValue(Uniforms::Transform::_PROJECTION, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._Proj)))); CE();
    shader->setUniformValue(Uniforms::Transform::_VIEW, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._MV)))); CE();
    shader->setUniformValue(Uniforms::Transform::_SCALE, transforms._scale); CE();

    // Month
    shader->setUniformValue(Uniforms::Timing::_MONTH, month); CE();

    // Terrain heightmap texture
    {
        f->glActiveTexture(texture_unit); CE();
        shader->setUniformValue(Uniforms::Texture::_TERRAIN_HEIGHTMAP, texture_unit-GL_TEXTURE0); CE();
        terrain->getDrawableTerrain().bind();CE();
        texture_unit++;
    }

    // Water heightmap texture
    {
        f->glActiveTexture(texture_unit); CE();
        shader->setUniformValue(Uniforms::Texture::_WATER_HEIGHTMAP, texture_unit-GL_TEXTURE0); CE();
        terrain->getTerrainWater()->bind();CE();
        texture_unit++;
    }

    // Normals texture
    {
        f->glActiveTexture(texture_unit);CE();
        shader->setUniformValue(Uniforms::Texture::_NORMALS, texture_unit-GL_TEXTURE0); CE();
        f->glBindTexture(GL_TEXTURE_2D, terrain->getNormals()->getTextureUnit());CE();
        texture_unit++;
    }

    reset_overlays(shader);
    if(terrain->overlaySlope())
    {
        shader->setUniformValue(Uniforms::Overlay::_SLOPE, true); CE();
    }
    else if(terrain->overlayAltitude())
    {
        shader->setUniformValue(Uniforms::Overlay::_ALTITUDE, true); CE();
    }
    else if(terrain->overlayShade() && terrain->getShade()->isValid())
    {
        shader->setUniformValue(Uniforms::Overlay::_SHADE, true); CE();
        f->glActiveTexture(texture_unit);CE();
        shader->setUniformValue(Uniforms::Texture::_SHADE, texture_unit-GL_TEXTURE0); CE();
        terrain->getShade()->bind();CE();
        texture_unit++;
    }
    else if(terrain->overlayTemperature() && terrain->getTemperature()->isValid())
    {
        shader->setUniformValue(Uniforms::Overlay::_TEMPERATURE, true); CE();

        // June temperature
        {
            f->glActiveTexture(texture_unit);CE();
            shader->setUniformValue(Uniforms::Texture::_TEMPERATURE_JUN, texture_unit-GL_TEXTURE0); CE();
            terrain->getTemperature()->getJunTemperature()->bind();CE();
            texture_unit++;
        }
        {
            f->glActiveTexture(texture_unit);CE();
            shader->setUniformValue(Uniforms::Texture::_TEMPERATURE_DEC, texture_unit-GL_TEXTURE0); CE();
            terrain->getTemperature()->getDecTemperature()->bind();CE();
            texture_unit++;
        }
    }
    else if(terrain->overlayMinDailyIllumination() && terrain->getMinDailyIllumination()->isValid())
    {
        shader->setUniformValue(Uniforms::Overlay::_MIN_DAILY_ILLUMINATION, true); CE();
        f->glActiveTexture(texture_unit);CE();
        shader->setUniformValue(Uniforms::Texture::_MIN_DAILY_ILLUMINATION, texture_unit-GL_TEXTURE0); CE();
        terrain->getMinDailyIllumination()->bind();CE();
        texture_unit++;
    }
    else if(terrain->overlayMaxDailyIllumination() && terrain->getMaxDailyIllumination()->isValid())
    {
        shader->setUniformValue(Uniforms::Overlay::_MAX_DAILY_ILLUMINATION, true); CE();
        f->glActiveTexture(texture_unit);CE();
        shader->setUniformValue(Uniforms::Texture::_MAX_DAILY_ILLUMINATION, texture_unit-GL_TEXTURE0); CE();
        terrain->getMaxDailyIllumination()->bind();CE();
        texture_unit++;
    }

    // Heightmap maximum height
    shader->setUniformValue(Uniforms::Terrain::_MAX_HEIGHT, (GLfloat)(terrain->getMaxHeight(true))); CE();

    // Heightmap base height
    shader->setUniformValue(Uniforms::Terrain::_BASE_HEIGHT, (GLfloat)(terrain->getBaseHeight(true))); CE();

    // Terrain Material properties
    {
        shader->setUniformValueArray(Uniforms::Terrain::_MATERIAL_AMBIENT, glm::value_ptr(Terrain::MaterialProperties::_AMBIENT), 1, 4); CE();
        shader->setUniformValueArray(Uniforms::Terrain::_MATERIAL_DIFFUSE, glm::value_ptr(Terrain::MaterialProperties::_DIFFUSE), 1, 4); CE();
    }
    // Water Material properties
    {
        shader->setUniformValueArray(Uniforms::Water::_MATERIAL_AMBIENT, glm::value_ptr(TerrainWater::MaterialProperties::_AMBIENT), 1, 4); CE();
        shader->setUniformValueArray(Uniforms::Water::_MATERIAL_DIFFUSE, glm::value_ptr(TerrainWater::MaterialProperties::_DIFFUSE), 1, 4); CE();
    }

    // Sun Light
    shader->setUniformValueArray(Uniforms::Lighting::_DIFFUSE_COLOR, glm::value_ptr(sunlight_properties.m_diffuse_color), 1, 4); CE();
    shader->setUniformValueArray(Uniforms::Lighting::_AMBIENT_COLOR, glm::value_ptr(sunlight_properties.m_ambient_color), 1, 4); CE();
    shader->setUniformValueArray(Uniforms::Lighting::_POSITION, glm::value_ptr(sunlight_properties.getPosition()), 1, 4); CE();

    terrain->render();

    shader->release();
}

void Renderer::renderTerrainElements(QOpenGLShaderProgram * shader, const Transform & transforms, const std::vector<Asset*> & p_assets, Terrain * terrain)
{
    int texture_unit(GL_TEXTURE0); // TEXTURE_0 reserved for the heightmap
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    shader->bind();

    shader->setUniformValue(Uniforms::Transform::_PROJECTION, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._Proj)))); CE();
    shader->setUniformValue(Uniforms::Transform::_VIEW, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._MV)))); CE();

    // The heightmap texture
    {
        f->glActiveTexture(texture_unit); CE();
        shader->setUniformValue(Uniforms::Texture::_TERRAIN_HEIGHTMAP, texture_unit-GL_TEXTURE0); CE();
        terrain->getDrawableTerrain().bind();
        texture_unit++;
    }

    // Draw all the assets
    for(Asset * asset : p_assets)
    {
        shader->setUniformValue(Uniforms::Assets::_USE_UNIFORM_COLOR, asset->m_uniform_color ); CE();

        if(asset->m_uniform_color) // Color data attached with verticies
        {
            glm::vec4 color(asset->m_color);
            shader->setUniformValueArray(Uniforms::Assets::_UNIFORM_COLOR, glm::value_ptr(color), 1, 4 ); CE();
        }

        for(Asset::AssetTransformations asset_transform : asset->getTransformations())
        {
            shader->setUniformValue(Uniforms::Transform::_MODEL, QMatrix4x4(glm::value_ptr(glm::transpose(asset_transform.mtw)))); CE();
            shader->setUniformValue(Uniforms::Transform::_SCALE, asset_transform.scale); CE();

            asset->render();
        }
    }

    shader->release();
}

void Renderer::renderAssets(QOpenGLShaderProgram * shader, const Transform & transforms, const std::vector<Asset*> & p_assets)
{
    shader->bind();

    shader->setUniformValue(Uniforms::Transform::_PROJECTION, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._Proj)))); CE();
    shader->setUniformValue(Uniforms::Transform::_VIEW, QMatrix4x4(glm::value_ptr(glm::transpose(transforms._MV)))); CE();;

    // Draw all the assets
    for(Asset * asset : p_assets)
    {
        shader->setUniformValue(Uniforms::Assets::_USE_UNIFORM_COLOR, asset->m_uniform_color ); CE();

        if(asset->m_uniform_color) // Color data attached with verticies
        {
            glm::vec4 color(asset->m_color);
            shader->setUniformValueArray(Uniforms::Assets::_UNIFORM_COLOR, glm::value_ptr(color), 1, 4 ); CE();
        }

        for(Asset::AssetTransformations asset_transform : asset->getTransformations())
        {
            shader->setUniformValue(Uniforms::Transform::_MODEL, QMatrix4x4(glm::value_ptr(glm::transpose(asset_transform.mtw)))); CE();
            shader->setUniformValue(Uniforms::Transform::_SCALE, asset_transform.scale); CE();

            asset->render();
        }
    }

    shader->release();
}

void Renderer::tmp_function1(WaterFluxGeneratorShader * shader, Terrain * terrain)
{
    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    shader->bind();  CE();

    /*******************
     * WATER HEIGHTMAP *
     *******************/
    f->glBindImageTexture(0, terrain->getTerrainWater()->textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);  CE();
    shader->setUniformValue(Uniforms::Texture::_WATER_HEIGHTMAP, 0);CE();

    /*********************
     * TERRAIN HEIGHTMAP *
     *********************/
    f->glBindImageTexture(1, terrain->getDrawableTerrain().textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);  CE();
    shader->setUniformValue(Uniforms::Texture::_TERRAIN_HEIGHTMAP, 1);CE();

    // Calculate the number of groups necessary
    GLuint n_groups_x(std::ceil(((float)terrain->getWidth()) / WaterFluxGeneratorShader::_GROUP_SIZE_X));
    GLuint n_groups_y(std::ceil(((float)terrain->getDepth()) / WaterFluxGeneratorShader::_GROUP_SIZE_Y));
    GLuint n_groups_z(1);

    f->glDispatchCompute(n_groups_x, n_groups_y, n_groups_z);  CE();
    f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();
    shader->release();

    // Trigger a refresh from the GPU
    terrain->getTerrainWater()->synchronize_from_GPU();
}

void Renderer::reset_overlays(QOpenGLShaderProgram * shader)
{
    shader->setUniformValue(Uniforms::Overlay::_SLOPE, false); CE();
    shader->setUniformValue(Uniforms::Overlay::_ALTITUDE, false); CE();
    shader->setUniformValue(Uniforms::Overlay::_SHADE, false); CE();
    shader->setUniformValue(Uniforms::Overlay::_TEMPERATURE, false); CE();
    shader->setUniformValue(Uniforms::Overlay::_MIN_DAILY_ILLUMINATION, false); CE();
    shader->setUniformValue(Uniforms::Overlay::_MAX_DAILY_ILLUMINATION, false); CE();
}

