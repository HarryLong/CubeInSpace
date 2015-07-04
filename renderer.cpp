#include "renderer.h"
#include "glheader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include "shader_programs.h"

Renderer::Renderer()
{
    init();
}

Renderer::~Renderer()
{

}

void Renderer::init()
{
    init_uniforms();
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
        shader->setUniformValue(m_texture_uniforms[TERRAIN_HEIGHT_MAP_TEXTURE], texture_unit-GL_TEXTURE0); CE();    // Terrain size
        terrain->getDrawableTerrain().bind();
        texture_unit++;
    }

    GLfloat imgDims[2] = {float(terrain->getWidth()), float(terrain->getDepth())};
    shader->setUniformValueArray(m_terrain_uniforms[TERRAIN_SIZE], imgDims, 1, 2); CE();

    // Calculate normals
    terrain->renderNormals(); CE();

    // reset viewport
    f->glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);

    shader->release(); CE();
}

void Renderer::renderTerrain(QOpenGLShaderProgram * shader, const Transform & transforms, Terrain * terrain, const LightProperties & sunlight_properties)
{
    int texture_unit(GL_TEXTURE0); // TEXTURE_0 reserved for the heightmap
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    shader->bind();

    shader->setUniformValue(m_transformation_uniforms[PROJECTION_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(transforms._Proj)))); CE();
    shader->setUniformValue(m_transformation_uniforms[VIEW_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(transforms._MV)))); CE();
    shader->setUniformValue(m_transformation_uniforms[SCALE], terrain->getScale()); CE();

    // Terrain heightmap texture
    {
        f->glActiveTexture(texture_unit); CE();
        shader->setUniformValue(m_texture_uniforms[TERRAIN_HEIGHT_MAP_TEXTURE], texture_unit-GL_TEXTURE0); CE();
        terrain->getDrawableTerrain().bind();CE();
        texture_unit++;
    }

    // Water heightmap texture
    {
        f->glActiveTexture(texture_unit); CE();
        shader->setUniformValue(m_texture_uniforms[WATER_HEIGHT_MAP_TEXTURE], texture_unit-GL_TEXTURE0); CE();
        terrain->getTerrainWater()->bind();CE();
        texture_unit++;
    }

    // Normals texture
    {
        f->glActiveTexture(texture_unit);CE();
        shader->setUniformValue(m_texture_uniforms[NORMALS_TEXTURE], texture_unit-GL_TEXTURE0); CE();
        f->glBindTexture(GL_TEXTURE_2D, terrain->getNormals()->getTextureUnit());CE();
        texture_unit++;
    }

    reset_overlays(shader);
    if(terrain->overlaySlope())
    {
        shader->setUniformValue(m_overlay_uniforms[SLOPE_OVERLAY], true); CE();
    }
    else if(terrain->overlayAltitude())
    {
        shader->setUniformValue(m_overlay_uniforms[ALTITUDE_OVERLAY], true); CE();
    }
    else if(terrain->overlayShade() && terrain->getShade()->isValid())
    {
        shader->setUniformValue(m_overlay_uniforms[SHADE_OVERLAY], true); CE();
        f->glActiveTexture(texture_unit);CE();
        shader->setUniformValue(m_texture_uniforms[SHADE_TEXTURE], texture_unit-GL_TEXTURE0); CE();
        terrain->getShade()->bind();CE();
        texture_unit++;
    }
    else if(terrain->overlayMinTemp() && terrain->getMinTemp()->isValid())
    {
        shader->setUniformValue(m_overlay_uniforms[TEMPERATURE_MIN], true); CE();
        f->glActiveTexture(texture_unit);CE();
        shader->setUniformValue(m_texture_uniforms[MIN_TEMPERATURE_TEXTURE], texture_unit-GL_TEXTURE0); CE();
        terrain->getMinTemp()->bind();CE();
        texture_unit++;
    }
    else if(terrain->overlayMaxTemp() && terrain->getMaxTemp()->isValid())
    {
        shader->setUniformValue(m_overlay_uniforms[TEMPERATURE_MAX], true); CE();
        f->glActiveTexture(texture_unit);CE();
        shader->setUniformValue(m_texture_uniforms[MAX_TEMPERATURE_TEXTURE], texture_unit-GL_TEXTURE0); CE();
        terrain->getMaxTemp()->bind();CE();
        texture_unit++;
    }
    else if(terrain->overlayMinDailyIllumination() && terrain->getMinDailyIllumination()->isValid())
    {
        shader->setUniformValue(m_overlay_uniforms[DAILY_ILLUMINATION_MIN], true); CE();
        f->glActiveTexture(texture_unit);CE();
        shader->setUniformValue(m_texture_uniforms[MIN_DAILY_ILLUMINATION_TEXTURE], texture_unit-GL_TEXTURE0); CE();
        terrain->getMinDailyIllumination()->bind();CE();
        texture_unit++;
    }
    else if(terrain->overlayMaxDailyIllumination() && terrain->getMaxDailyIllumination()->isValid())
    {
        shader->setUniformValue(m_overlay_uniforms[DAILY_ILLUMINATION_MAX], true); CE();
        f->glActiveTexture(texture_unit);CE();
        shader->setUniformValue(m_texture_uniforms[MAX_DAILY_ILLUMINATION_TEXTURE], texture_unit-GL_TEXTURE0); CE();
        terrain->getMaxDailyIllumination()->bind();CE();
        texture_unit++;
    }

    // Heightmap maximum height
    shader->setUniformValue(m_terrain_uniforms[MAX_HEIGHT], (GLfloat)(terrain->getMaxHeight(true))); CE();

    // Heightmap base height
    shader->setUniformValue(m_terrain_uniforms[BASE_HEIGHT], (GLfloat)(terrain->getBaseHeight(true))); CE();

    // Terrain Material properties
    {
        shader->setUniformValueArray(m_terrain_uniforms[TERRAIN_MATERIAL_AMBIENT], glm::value_ptr(Terrain::MaterialProperties::_AMBIENT), 1, 4); CE();
        shader->setUniformValueArray(m_terrain_uniforms[TERRAIN_MATERIAL_DIFFUSE], glm::value_ptr(Terrain::MaterialProperties::_DIFFUSE), 1, 4); CE();
    }
    // Water Material properties
    {
        shader->setUniformValueArray(m_water_uniforms[WATER_MATERIAL_AMBIENT], glm::value_ptr(TerrainWater::MaterialProperties::_AMBIENT), 1, 4); CE();
        shader->setUniformValueArray(m_water_uniforms[WATER_MATERIAL_DIFFUSE], glm::value_ptr(TerrainWater::MaterialProperties::_DIFFUSE), 1, 4); CE();
    }

    // Sun Light
    shader->setUniformValueArray(m_lighting_uniforms[LIGHT_DIFFUSE_COLOR], glm::value_ptr(sunlight_properties.m_diffuse_color), 1, 4); CE();
    shader->setUniformValueArray(m_lighting_uniforms[LIGHT_AMBIENT_COLOR], glm::value_ptr(sunlight_properties.m_ambient_color), 1, 4); CE();
    shader->setUniformValueArray(m_lighting_uniforms[LIGHT_POS], glm::value_ptr(sunlight_properties.getPosition()), 1, 4); CE();

    terrain->render();

    shader->release();
}

void Renderer::renderTerrainElements(QOpenGLShaderProgram * shader, const Transform & transforms, const std::vector<Asset*> & p_assets, Terrain * terrain)
{
    int texture_unit(GL_TEXTURE0); // TEXTURE_0 reserved for the heightmap
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    shader->bind();

    // The transformation matrices
    shader->setUniformValue(m_transformation_uniforms[PROJECTION_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(transforms._Proj)))); CE();
    shader->setUniformValue(m_transformation_uniforms[VIEW_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(transforms._MV)))); CE();
    shader->setUniformValue(m_transformation_uniforms[SCALE], transforms._scale); CE();

    // The heightmap texture
    {
        f->glActiveTexture(texture_unit); CE();
        shader->setUniformValue(m_texture_uniforms[TERRAIN_HEIGHT_MAP_TEXTURE], texture_unit-GL_TEXTURE0); CE();
        terrain->getDrawableTerrain().bind();
        texture_unit++;
    }

    // Draw all the assets
    for(Asset * asset : p_assets)
    {
        shader->setUniformValue(m_asset_uniforms[USE_UNIFORM_COLOR], asset->m_uniform_color ); CE();

        if(asset->m_uniform_color) // Color data attached with verticies
        {
            glm::vec4 color(asset->m_color);
            shader->setUniformValueArray(m_asset_uniforms[UNIFORM_COLOR], glm::value_ptr(color), 1, 4 ); CE();
        }

        for(Asset::AssetTransformations asset_transform : asset->getTransformations())
        {
            shader->setUniformValue(m_transformation_uniforms[MTW_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(asset_transform.mtw)))); CE();

            asset->render();
        }
    }

    shader->release();
}

void Renderer::renderAssets(QOpenGLShaderProgram * shader, const Transform & transforms, const std::vector<Asset*> & p_assets)
{
    shader->bind();

    shader->setUniformValue(m_transformation_uniforms[PROJECTION_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(transforms._Proj)))); CE();
    shader->setUniformValue(m_transformation_uniforms[VIEW_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(transforms._MV)))); CE();

    // Draw all the assets
    for(Asset * asset : p_assets)
    {
        if(asset->m_uniform_color) // Color data attached with verticies
        {
            glm::vec4 color(asset->m_color);
            shader->setUniformValueArray(m_asset_uniforms[UNIFORM_COLOR], glm::value_ptr(color), 1, 4 ); CE();
        }
        shader->setUniformValue(m_asset_uniforms[USE_UNIFORM_COLOR], asset->m_uniform_color ); CE();

        for(Asset::AssetTransformations asset_transform : asset->getTransformations())
        {
            shader->setUniformValue(m_transformation_uniforms[MTW_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(asset_transform.mtw)))); CE();
            shader->setUniformValue(m_transformation_uniforms[SCALE], asset_transform.scale); CE();

            asset->render();
        }
    }

    shader->release();
}

void Renderer::tmp_function1(WaterFluxGeneratorShader * shader, Terrain * terrain)
{
//    terrain->getTerrainWater()->tmp_calculateTotalWater();

    QOpenGLFunctions_4_3_Core * f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if(!f)
        qCritical() << "Could not obtain required OpenGL context version";
    f->initializeOpenGLFunctions();

    shader->bind();  CE();

    if(terrain->getTerrainWater()->width() != terrain->getDrawableTerrain().width())
        qCritical() << "TEXTURES AREN'T THE SAME WIDTH!";
    if(terrain->getTerrainWater()->height() != terrain->getDrawableTerrain().height())
        qCritical() << "TEXTURES AREN'T THE SAME HEIGHT!";

    /*******************
     * WATER HEIGHTMAP *
     *******************/
    f->glBindImageTexture(0, terrain->getTerrainWater()->textureId(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);  CE();
    shader->setUniformValue(WaterFluxGeneratorShader::Uniforms::_WATER_HEIGHTMAP.toStdString().c_str(), 0);CE();

    /*********************
     * TERRAIN HEIGHTMAP *
     *********************/
    f->glBindImageTexture(1, terrain->getDrawableTerrain().textureId(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32F);  CE();
    shader->setUniformValue(WaterFluxGeneratorShader::Uniforms::_TERRAIN_HEIGHTMAP.toStdString().c_str(), 1);CE();

    // Calculate the number of groups necessary
    GLuint n_groups_x(std::ceil(((float)terrain->getWidth()) / WaterFluxGeneratorShader::_GROUP_SIZE_X));
    GLuint n_groups_y(std::ceil(((float)terrain->getDepth()) / WaterFluxGeneratorShader::_GROUP_SIZE_Y));
    GLuint n_groups_z(1);

//    GLuint n_groups_x(1);
//    GLuint n_groups_y(1);
//    GLuint n_groups_z(1);

    qCritical() << "N groups x: " << n_groups_x;
    qCritical() << "N groups y: " << n_groups_y;
    qCritical() << "N groups z: " << n_groups_z;

    f->glDispatchCompute(n_groups_x, n_groups_y, n_groups_z);  CE();
    f->glMemoryBarrier(GL_ALL_BARRIER_BITS);  CE();
    shader->release();

    // Trigger a refresh from the GPU
    terrain->getTerrainWater()->synchronize_from_GPU();
    terrain->getDrawableTerrain().synchronize_from_GPU();

//    terrain->getTerrainWater()->tmp_calculateTotalWater();
}

void Renderer::reset_overlays(QOpenGLShaderProgram * shader)
{
    shader->setUniformValue(m_overlay_uniforms[SLOPE_OVERLAY], false); CE();
    shader->setUniformValue(m_overlay_uniforms[ALTITUDE_OVERLAY], false); CE();
    shader->setUniformValue(m_overlay_uniforms[SHADE_OVERLAY], false); CE();
    shader->setUniformValue(m_overlay_uniforms[TEMPERATURE_MIN], false); CE();
    shader->setUniformValue(m_overlay_uniforms[TEMPERATURE_MAX], false); CE();
    shader->setUniformValue(m_overlay_uniforms[DAILY_ILLUMINATION_MIN], false); CE();
    shader->setUniformValue(m_overlay_uniforms[DAILY_ILLUMINATION_MAX], false); CE();
}

void Renderer::init_uniforms()
{
    // Transformation uniforms
    m_transformation_uniforms[TransformationUniforms::PROJECTION_MAT] = "transform.projMat";
    m_transformation_uniforms[TransformationUniforms::VIEW_MAT] = "transform.viewMat";
    m_transformation_uniforms[TransformationUniforms::MTW_MAT] = "transform.mtwMat";
    m_transformation_uniforms[TransformationUniforms::SCALE] = "transform.scale";

    // Terrain uniforms
    m_terrain_uniforms[TerrainUniforms::MAX_HEIGHT] = "max_height";
    m_terrain_uniforms[TerrainUniforms::BASE_HEIGHT] = "base_height";
    m_terrain_uniforms[TerrainUniforms::TERRAIN_SIZE] = "terrain_size";
    m_terrain_uniforms[TerrainUniforms::TERRAIN_MATERIAL_AMBIENT] = "terrain_material_ambient";
    m_terrain_uniforms[TerrainUniforms::TERRAIN_MATERIAL_DIFFUSE] = "terrain_material_diffuse";

    // Texture uniforms
    m_texture_uniforms[TextureUniforms::TERRAIN_HEIGHT_MAP_TEXTURE] = "terrain_height_map_texture";
    m_texture_uniforms[TextureUniforms::WATER_HEIGHT_MAP_TEXTURE] = "water_height_map_texture";
    m_texture_uniforms[TextureUniforms::NORMALS_TEXTURE] = "normals_texture";
    m_texture_uniforms[TextureUniforms::SHADE_TEXTURE] = "shade_texture";
    m_texture_uniforms[TextureUniforms::MIN_TEMPERATURE_TEXTURE] = "min_temp_texture";
    m_texture_uniforms[TextureUniforms::MAX_TEMPERATURE_TEXTURE] = "max_temp_texture";
    m_texture_uniforms[TextureUniforms::MIN_DAILY_ILLUMINATION_TEXTURE] = "min_daily_illumination_texture";
    m_texture_uniforms[TextureUniforms::MAX_DAILY_ILLUMINATION_TEXTURE] = "max_daily_illumination_texture";

    // Overlay options uniforms
    m_overlay_uniforms[TerrainOverlayUniforms::OVERLAY_DISABLED] = "overlay.none";
    m_overlay_uniforms[TerrainOverlayUniforms::SLOPE_OVERLAY] = "overlay.slope";
    m_overlay_uniforms[TerrainOverlayUniforms::ALTITUDE_OVERLAY] = "overlay.altitude";
    m_overlay_uniforms[TerrainOverlayUniforms::SHADE_OVERLAY] = "overlay.shade";
    m_overlay_uniforms[TerrainOverlayUniforms::TEMPERATURE_MIN] = "overlay.temperature_min";
    m_overlay_uniforms[TerrainOverlayUniforms::TEMPERATURE_MAX] = "overlay.temperature_max";
    m_overlay_uniforms[TerrainOverlayUniforms::DAILY_ILLUMINATION_MIN] = "overlay.daily_illumination_min";
    m_overlay_uniforms[TerrainOverlayUniforms::DAILY_ILLUMINATION_MAX] = "overlay.daily_illumination_max";

    // Lighting uniforms
    m_lighting_uniforms[LightingUniforms::LIGHT_POS] = "light_position";
    m_lighting_uniforms[LightingUniforms::LIGHT_DIFFUSE_COLOR] = "light_diffuse_color";
    m_lighting_uniforms[LightingUniforms::LIGHT_AMBIENT_COLOR] = "light_ambient_color";

    // Option uniforms
    m_options_uniforms[OptionUniforms::DRAWING_GRID] = "drawing_grid";

    // Asset uniforms
    m_asset_uniforms[AssetUniforms::UNIFORM_COLOR] = "uniform_color";
    m_asset_uniforms[AssetUniforms::USE_UNIFORM_COLOR] = "use_uniform_color";

    // Water uniforms
    m_water_uniforms[WaterUniforms::WATER_MATERIAL_AMBIENT] = "water_material_ambient";
    m_water_uniforms[WaterUniforms::WATER_MATERIAL_DIFFUSE] = "water_material_diffuse";
}
