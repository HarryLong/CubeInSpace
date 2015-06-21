#include "renderer.h"
#include "glheader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "view_manager.h"
#include <iostream>
#include <QGLShaderProgram>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

Renderer::Renderer()
{
    init();
}

Renderer::~Renderer()
{
//    for(auto it = m_shaders.begin(); it != m_shaders.end(); it++)
//    {
//        delete it->second;
//    }
}

void Renderer::init()
{
    init_uniforms();
//    init_shaders();
}

void Renderer::append_shader(const ShaderType & shader_type, const QString & description)
{
//    // Compile and link the shader
//    ShaderProgram * sp(new ShaderProgram(g_fragment_shader_files[shader_type], g_vertex_shader_files[shader_type], description));
//    if(!sp->compileAndLink())
//        std::cerr << "Failed to compile shader: " << sp->getDescription().toStdString() << std::endl;
//    else // Insert the shader
//        m_shaders.insert( std::pair<ShaderType, ShaderProgram *>(shader_type, sp) );
}

void Renderer::init_shaders()
{
    append_shader(ShaderType::BASE, "Base Shader");
    append_shader(ShaderType::TERRAIN, "Terrain Shader");
    append_shader(ShaderType::TERRAIN_ELEMENTS, "Terrain Elements Shader");
    append_shader(ShaderType::NORMALS, "Normals Generator Shader");
}

void Renderer::calculateNormals(QGLShaderProgram * shader, Terrain &terrain)
{
    int texture_unit(0); // TEXTURE_0 reserved for the heightmap

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    shader->bind(); CE();

    // Save the viewport
    GLint viewport[4];
    f->glGetIntegerv(GL_VIEWPORT, viewport); // save current viewport
    // reset current viewport
    f->glViewport(0,0,terrain.getWidth(), terrain.getDepth()); CE();

    // The heightmap texture
    shader->setUniformValue(m_texture_uniforms[HEIGHT_MAP_TEXTURE], texture_unit); CE();    // Terrain size
    terrain.getDrawableTerrain().bind();

    GLfloat imgDims[2] = {float(terrain.getWidth()), float(terrain.getDepth())};
    shader->setUniformValueArray(m_terrain_uniforms[TERRAIN_SIZE], imgDims, 1, 2); CE();

    // Calculate normals
    terrain.renderNormals(); CE();

    // reset viewport
    f->glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);

    shader->release(); CE();
}

void Renderer::renderTerrain(QGLShaderProgram * shader, const ViewManager & p_view, Terrain& terrain, const LightProperties & sunlight_properties)
{
    int texture_unit(0); // TEXTURE_0 reserved for the heightmap
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    shader->bind();

    Transform transform(p_view.getProjMtx(), p_view.getViewMatrix());

    shader->setUniformValue(m_transformation_uniforms[PROJECTION_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(transform.m_projection_mat)))); CE();
    shader->setUniformValue(m_transformation_uniforms[VIEW_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(transform.m_view_mat)))); CE();
    shader->setUniformValue(m_transformation_uniforms[SCALE], terrain.getScale()); CE();

    // The heightmap texture
    shader->setUniformValue(m_texture_uniforms[HEIGHT_MAP_TEXTURE], texture_unit); CE();
    terrain.getDrawableTerrain().bind();

    // Normals texture
    if(terrain.overlayNone() || terrain.overlaySlope())
    {
       shader->setUniformValue(m_texture_uniforms[NORMALS_TEXTURE], texture_unit++); CE();
        f->glBindTexture(GL_TEXTURE_2D, terrain.getNormals().getTextureUnit());
    }
    else if(terrain.overlayShade() && terrain.getShade().isValid())
    {
        shader->setUniformValue(m_texture_uniforms[SHADE_TEXTURE], texture_unit++); CE();
        terrain.getShade().bind();
    }
    else if(terrain.overlayMinTemp() && terrain.getMinTemp().isValid())
    {
        shader->setUniformValue(m_texture_uniforms[MIN_TEMPERATURE_TEXTURE], texture_unit++); CE();
        terrain.getMinTemp().bind();
    }
    else if(terrain.overlayMaxTemp() && terrain.getMaxTemp().isValid())
    {
        shader->setUniformValue(m_texture_uniforms[MAX_TEMPERATURE_TEXTURE], texture_unit++); CE();
        terrain.getMaxTemp().bind();
    }
    else if(terrain.overlayMinDailyIllumination() && terrain.getMinDailyIllumination().isValid())
    {
        shader->setUniformValue(m_texture_uniforms[MIN_DAILY_ILLUMINATION_TEXTURE], texture_unit++); CE();
        terrain.getMinDailyIllumination().bind();
    }
    else if(terrain.overlayMaxDailyIllumination() && terrain.getMaxDailyIllumination().isValid())
    {
        shader->setUniformValue(m_texture_uniforms[MAX_DAILY_ILLUMINATION_TEXTURE], texture_unit++); CE();
        terrain.getMaxDailyIllumination().bind();
    }

    // Heightmap maximum height
    shader->setUniformValue(m_terrain_uniforms[MAX_HEIGHT], (GLfloat)(terrain.getMaxHeight(true))); CE();

    // Heightmap base height
    shader->setUniformValue(m_terrain_uniforms[BASE_HEIGHT], (GLfloat)(terrain.getBaseHeight(true))); CE();

    // Material properties
    const TerrainMaterialProperties & terrain_material_properties (terrain.getMaterialProperties());
    shader->setUniformValueArray(m_terrain_uniforms[MATERIAL_DIFFUSE], glm::value_ptr(terrain_material_properties.diffuse), 1, 4); CE();
    shader->setUniformValueArray(m_terrain_uniforms[MATERIAL_AMBIENT], glm::value_ptr(terrain_material_properties.ambient), 1, 4); CE();

    // Overlay
    shader->setUniformValue(m_overlay_uniforms[OVERLAY_DISABLED], terrain.overlayNone()); CE();
    shader->setUniformValue(m_overlay_uniforms[SLOPE_OVERLAY], terrain.overlaySlope()); CE();
    shader->setUniformValue(m_overlay_uniforms[ALTITUDE_OVERLAY], terrain.overlayAltitude()); CE();
    shader->setUniformValue(m_overlay_uniforms[SHADE_OVERLAY], terrain.overlayShade()); CE();
    shader->setUniformValue(m_overlay_uniforms[TEMPERATURE_MIN], terrain.overlayMinTemp()); CE();
    shader->setUniformValue(m_overlay_uniforms[TEMPERATURE_MAX], terrain.overlayMaxTemp()); CE();
    shader->setUniformValue(m_overlay_uniforms[DAILY_ILLUMINATION_MIN], terrain.overlayMinDailyIllumination()); CE();
    shader->setUniformValue(m_overlay_uniforms[DAILY_ILLUMINATION_MAX], terrain.overlayMaxDailyIllumination()); CE();

    // Sun Light
    shader->setUniformValueArray(m_lighting_uniforms[LIGHT_DIFFUSE_COLOR], glm::value_ptr(sunlight_properties.m_diffuse_color), 1, 4); CE();
    shader->setUniformValueArray(m_lighting_uniforms[LIGHT_AMBIENT_COLOR], glm::value_ptr(sunlight_properties.m_ambient_color), 1, 4); CE();
    shader->setUniformValueArray(m_lighting_uniforms[LIGHT_POS], glm::value_ptr(sunlight_properties.getPosition()), 1, 4); CE();

    terrain.render();

    shader->release();
}

void Renderer::renderTerrainElements(QGLShaderProgram * shader, const ViewManager & p_view, const std::vector<Asset*> & p_assets, Terrain& terrain)
{
    int texture_unit(0); // TEXTURE_0 reserved for the heightmap

    shader->bind();

    // The transformation matrices
    Transform transform(p_view.getProjMtx(), p_view.getViewMatrix());
    shader->setUniformValue(m_transformation_uniforms[PROJECTION_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(transform.m_projection_mat)))); CE();
    shader->setUniformValue(m_transformation_uniforms[VIEW_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(transform.m_view_mat)))); CE();
    shader->setUniformValue(m_transformation_uniforms[SCALE], terrain.getScale()); CE();

    // The heightmap texture
    shader->setUniformValue(m_texture_uniforms[HEIGHT_MAP_TEXTURE], texture_unit); CE();
    terrain.getDrawableTerrain().bind();

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
            transform.m_mtw_mat = asset_transform.mtw;

            shader->setUniformValue(m_transformation_uniforms[MTW_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(transform.m_mtw_mat)))); CE();

            asset->render();
        }
    }

    shader->release();
}

void Renderer::renderAssets(QGLShaderProgram * shader, const ViewManager & p_view, const std::vector<Asset*> & p_assets)
{
    shader->bind();

    Transform transform(p_view.getProjMtx(), p_view.getViewMatrix());
    shader->setUniformValue(m_transformation_uniforms[PROJECTION_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(transform.m_projection_mat)))); CE();
    shader->setUniformValue(m_transformation_uniforms[VIEW_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(transform.m_view_mat)))); CE();
    int i (0);
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
            transform.m_mtw_mat = asset_transform.mtw;
            transform.m_scale = asset_transform.scale;

            shader->setUniformValue(m_transformation_uniforms[MTW_MAT], QMatrix4x4(glm::value_ptr(glm::transpose(transform.m_mtw_mat)))); CE();
            shader->setUniformValue(m_transformation_uniforms[SCALE], transform.m_scale); CE();

            asset->render();
        }
    }

    shader->release();
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
    m_terrain_uniforms[TerrainUniforms::MATERIAL_DIFFUSE] = "material_diffuse";
    m_terrain_uniforms[TerrainUniforms::MATERIAL_AMBIENT] = "material_ambient";

    // Texture uniforms
    m_texture_uniforms[TextureUniforms::HEIGHT_MAP_TEXTURE] = "height_map_texture";
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
}

void Renderer::printShaders()
{
    std::cout << "******************" << std::endl;
    std::cout << "* SHADER PROGRAMS*" << std::endl;
    std::cout << "******************" << std::endl;
//    for(auto it = m_shaders.begin(); it != m_shaders.end(); it++)
//    {
//        ShaderProgram * sp = it->second;
//        std::cout << "  - " << sp->getDescription().toStdString() << " [ID: " << sp->getProgramID() << "]" << std::endl;
//    }
    std::cout << "******************" << std::endl;
}
