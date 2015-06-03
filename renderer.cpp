#include "renderer.h"
#include "glheader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "view_manager.h"
#include <iostream>

Renderer::Renderer() : m_active_terrain_overlay(OVERLAY_DISABLED)
{

}

Renderer::~Renderer()
{
    for(auto it = m_shaders.begin(); it != m_shaders.end(); it++)
    {
        delete it->second;
    }
}

void Renderer::init()
{
    init_uniforms();
    init_shaders();
}

void Renderer::append_shader(const ShaderType & shader_type, const QString & description)
{
    // Compile and link the shader
    ShaderProgram * sp(new ShaderProgram(g_fragment_shader_files[shader_type], g_vertex_shader_files[shader_type], description));
    if(!sp->compileAndLink())
        std::cerr << "Failed to compile shader: " << sp->getDescription().toStdString() << std::endl;
    else // Insert the shader
        m_shaders.insert( std::pair<ShaderType, ShaderProgram *>(shader_type, sp) );
}

void Renderer::init_shaders()
{
    append_shader(ShaderType::BASE, "Base Shader");
    append_shader(ShaderType::TERRAIN, "Terrain Shader");
    append_shader(ShaderType::TERRAIN_ELEMENTS, "Terrain Elements Shader");
    append_shader(ShaderType::NORMALS, "Normals Generator Shader");
}

void Renderer::renderTerrain(const ViewManager & p_view, Terrain& terrain, const LightProperties & sunlight_properties)
{
    if(!terrain.getNormals().valid()) // Need to recalculate normals
    {
        // Save the viewport
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport); // save current viewport
        // reset current viewport
        glViewport(0,0,terrain.getWidth(), terrain.getDepth()); CE();

        GLuint prog_id = m_shaders[NORMALS]->getProgramID();
        glUseProgram(prog_id);
        // Set uniforms
        GLuint heightmap_texture = glGetUniformLocation(prog_id, m_terrain_uniforms[HEIGHT_MAP_TEXTURE]); CE();
        glUniform1i(heightmap_texture, (GLint)(terrain.getHeightMapTextureUnit() - GL_TEXTURE0));  CE(); // assumes texture unit 0 is bound to heightmap texture

        GLfloat imgDims[2] = {float(terrain.getWidth()), float(terrain.getDepth())};
        glUniform2fv(glGetUniformLocation(prog_id, m_terrain_uniforms[TERRAIN_SIZE]), 1, imgDims); CE();

        // Calculate normals
        terrain.getNormals().render();

        glUseProgram(0);  CE();

        // reset viewport
        glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);

        terrain.normalsRecalculated();
    }

    GLuint prog_id = m_shaders[TERRAIN]->getProgramID();

    glUseProgram(prog_id); CE()

    Transform transform(p_view.getProjMtx(), p_view.getViewMatrix());
    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_projection_mat)); CE();
    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_view_mat)); CE();

    // Heightmap texture
    GLuint heightmap_texture = glGetUniformLocation(prog_id, m_terrain_uniforms[HEIGHT_MAP_TEXTURE]); CE();
    glUniform1i(heightmap_texture, (GLint)(terrain.getHeightMapTextureUnit() - GL_TEXTURE0));  CE(); // assumes texture unit 0 is bound to heightmap texture

    // Normals texture
    if(m_active_terrain_overlay == OVERLAY_DISABLED || m_active_terrain_overlay == SLOPE_OVERLAY)
    {
        GLuint normals_texture = glGetUniformLocation(prog_id, m_terrain_uniforms[NORMALS_TEXTURE]); CE();
        glUniform1i(normals_texture, (GLint)(terrain.getNormals().getNormalsTextureUnit() - GL_TEXTURE0));  CE(); // assumes texture unit 0 is bound to heightmap texture
    }

    // Slope texture
    if(m_active_terrain_overlay == SHADE_OVERLAY)
    {
        GLuint shade_texture = glGetUniformLocation(prog_id, m_terrain_uniforms[SHADE_TEXTURE]); CE();
        glUniform1i(shade_texture, (GLint)(terrain.getShadeTextureUnit() - GL_TEXTURE0));  CE(); // assumes texture unit 0 is bound to heightmap texture
    }

    // Heightmap maximum height
    glUniform1f(glGetUniformLocation(prog_id, m_terrain_uniforms[MAX_HEIGHT]), (GLfloat) terrain.getMaxHeight() ); CE();

    // Heightmap base height
    glUniform1f(glGetUniformLocation(prog_id, m_terrain_uniforms[BASE_HEIGHT]), (GLfloat) terrain.getBaseHeight()); CE();

    // Material properties
    const TerrainMaterialProperties & terrain_material_properties (terrain.getMaterialProperties());
    glUniform4fv(glGetUniformLocation(prog_id, m_terrain_uniforms[MATERIAL_DIFFUSE]), 1, glm::value_ptr(terrain_material_properties.diffuse)); CE();
    glUniform4fv(glGetUniformLocation(prog_id, m_terrain_uniforms[MATERIAL_AMBIENT]), 1, glm::value_ptr(terrain_material_properties.ambient)); CE();

    // Overlay
    glUniform1i(glGetUniformLocation(prog_id, m_overlay_uniforms[OVERLAY_DISABLED]), m_active_terrain_overlay == OVERLAY_DISABLED); CE();
    glUniform1i(glGetUniformLocation(prog_id, m_overlay_uniforms[SLOPE_OVERLAY]), m_active_terrain_overlay == SLOPE_OVERLAY); CE();
    glUniform1i(glGetUniformLocation(prog_id, m_overlay_uniforms[ALTITUDE_OVERLAY]), m_active_terrain_overlay == ALTITUDE_OVERLAY); CE();
    glUniform1i(glGetUniformLocation(prog_id, m_overlay_uniforms[SHADE_OVERLAY]), m_active_terrain_overlay == SHADE_OVERLAY); CE();

    // Sun Light
    glUniform4fv(glGetUniformLocation(prog_id, m_lighting_uniforms[LIGHT_DIFFUSE_COLOR]), 1, glm::value_ptr(sunlight_properties.m_diffuse_color)); CE();
    glUniform4fv(glGetUniformLocation(prog_id, m_lighting_uniforms[LIGHT_AMBIENT_COLOR]), 1, glm::value_ptr(sunlight_properties.m_ambient_color)); CE();
    glUniform4fv(glGetUniformLocation(prog_id, m_lighting_uniforms[LIGHT_POS]), 1, glm::value_ptr(sunlight_properties.getPosition())); CE();

    terrain.render();

    glUseProgram(0); // unbind
}

void Renderer::renderTerrainElements(const ViewManager & p_view, const std::vector<const Asset*> & p_assets, GLint terrain_heightmap_texture_unit)
{
    GLuint prog_id = m_shaders[TERRAIN_ELEMENTS]->getProgramID();

    glUseProgram(prog_id); CE()

    // The transformation matrices
    Transform transform(p_view.getProjMtx(), p_view.getViewMatrix());
    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_projection_mat)); CE();
    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_view_mat)); CE();

    // The heightmap texture
    GLuint heightmap_texture = glGetUniformLocation(prog_id, m_terrain_uniforms[HEIGHT_MAP_TEXTURE]); CE();
    glUniform1i(heightmap_texture, terrain_heightmap_texture_unit - GL_TEXTURE0);  CE(); // assumes texture unit 0 is bound to heightmap texture

    // Draw all the assets
    for(const Asset * asset : p_assets)
    {
        transform.m_mtw_mat = asset->getMtwMat();

        glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[MTW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_mtw_mat)); CE();

        glUniform1i(glGetUniformLocation(prog_id, m_asset_uniforms[USE_UNIFORM_COLOR]), asset->m_uniform_color );

        if(asset->m_uniform_color) // Color data attached with verticies
        {
            glm::vec4 color(asset->m_color);
            glUniform4fv(glGetUniformLocation(prog_id, m_asset_uniforms[UNIFORM_COLOR]),1, glm::value_ptr(color) );
        }

        asset->render();
    }

    glBindVertexArray(0); // Unbind
    glUseProgram(0); // unbind
}

void Renderer::renderAssets(const ViewManager & p_view, const std::vector<const Asset*> & p_assets)
{
    GLuint prog_id = m_shaders[BASE]->getProgramID();

    glUseProgram(prog_id); CE()

    Transform transform(p_view.getProjMtx(), p_view.getViewMatrix());

    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_projection_mat)); CE();
    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_view_mat)); CE();

    // Draw all the assets
    for(const Asset * asset : p_assets)
    {
        transform.m_mtw_mat = asset->getMtwMat();
        transform.m_scale = asset->getScale();

        glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[MTW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_mtw_mat)); CE();
        glUniform1f(glGetUniformLocation(prog_id, m_transformation_uniforms[SCALE]), (GLfloat) transform.m_scale); CE();

        glUniform1i(glGetUniformLocation(prog_id, m_asset_uniforms[USE_UNIFORM_COLOR]), asset->m_uniform_color );

        if(asset->m_uniform_color) // Color data attached with verticies
        {
            glm::vec4 color(asset->m_color);
            glUniform4fv(glGetUniformLocation(prog_id, m_asset_uniforms[UNIFORM_COLOR]),1, glm::value_ptr(color) );
        }

         asset->render();
    }

    glUseProgram(0); // unbind
}

void Renderer::init_uniforms()
{
    // Transformation uniforms
    m_transformation_uniforms.insert(std::pair<TransformationUniforms,const char *>(TransformationUniforms::PROJECTION_MAT,"transform.projMat"));
    m_transformation_uniforms.insert(std::pair<TransformationUniforms,const char *>(TransformationUniforms::VIEW_MAT,"transform.viewMat"));
    m_transformation_uniforms.insert(std::pair<TransformationUniforms,const char *>(TransformationUniforms::MTW_MAT,"transform.mtwMat"));
    m_transformation_uniforms.insert(std::pair<TransformationUniforms,const char *>(TransformationUniforms::SCALE,"transform.scale"));

    // Terrain uniforms
    m_terrain_uniforms.insert(std::pair<TerrainUniforms,const char *>(TerrainUniforms::HEIGHT_MAP_TEXTURE,"height_map_texture"));
    m_terrain_uniforms.insert(std::pair<TerrainUniforms,const char *>(TerrainUniforms::NORMALS_TEXTURE,"normals_texture"));
    m_terrain_uniforms.insert(std::pair<TerrainUniforms,const char *>(TerrainUniforms::SHADE_TEXTURE,"shade_texture"));
    m_terrain_uniforms.insert(std::pair<TerrainUniforms,const char *>(TerrainUniforms::MAX_HEIGHT,"max_height"));
    m_terrain_uniforms.insert(std::pair<TerrainUniforms,const char *>(TerrainUniforms::BASE_HEIGHT,"base_height"));
    m_terrain_uniforms.insert(std::pair<TerrainUniforms,const char *>(TerrainUniforms::TERRAIN_SIZE,"terrain_size"));
    m_terrain_uniforms.insert(std::pair<TerrainUniforms,const char *>(TerrainUniforms::MATERIAL_DIFFUSE,"material_diffuse"));
    m_terrain_uniforms.insert(std::pair<TerrainUniforms,const char *>(TerrainUniforms::MATERIAL_AMBIENT,"material_ambient"));

    // Overlay options uniforms
    m_overlay_uniforms.insert(std::pair<TerrainOverlayUniforms,const char *>(TerrainOverlayUniforms::OVERLAY_DISABLED,"overlay.none"));
    m_overlay_uniforms.insert(std::pair<TerrainOverlayUniforms,const char *>(TerrainOverlayUniforms::SLOPE_OVERLAY,"overlay.slope"));
    m_overlay_uniforms.insert(std::pair<TerrainOverlayUniforms,const char *>(TerrainOverlayUniforms::ALTITUDE_OVERLAY,"overlay.altitude"));
    m_overlay_uniforms.insert(std::pair<TerrainOverlayUniforms,const char *>(TerrainOverlayUniforms::SHADE_OVERLAY,"overlay.shade"));

    // Lighting uniforms
    m_lighting_uniforms.insert(std::pair<LightingUniforms,const char *>(LightingUniforms::LIGHT_POS,"light_position"));
    m_lighting_uniforms.insert(std::pair<LightingUniforms,const char *>(LightingUniforms::LIGHT_DIFFUSE_COLOR,"light_diffuse_color"));
    m_lighting_uniforms.insert(std::pair<LightingUniforms,const char *>(LightingUniforms::LIGHT_AMBIENT_COLOR,"light_ambient_color"));

    // Option uniforms
    m_options_uniforms.insert(std::pair<OptionUniforms,const char *>(OptionUniforms::DRAWING_GRID,"drawing_grid"));

    // Asset uniforms
    m_asset_uniforms.insert(std::pair<AssetUniforms,const char *>(AssetUniforms::UNIFORM_COLOR, "uniform_color"));
    m_asset_uniforms.insert(std::pair<AssetUniforms,const char *>(AssetUniforms::USE_UNIFORM_COLOR, "use_uniform_color"));
}

void Renderer::setOverlay(TerrainOverlayUniforms overlay)
{
    m_active_terrain_overlay = overlay;
}

void Renderer::printShaders()
{
    std::cout << "******************" << std::endl;
    std::cout << "* SHADER PROGRAMS*" << std::endl;
    std::cout << "******************" << std::endl;
    for(auto it = m_shaders.begin(); it != m_shaders.end(); it++)
    {
        ShaderProgram * sp = it->second;
        std::cout << "  - " << sp->getDescription().toStdString() << " [ID: " << sp->getProgramID() << "]" << std::endl;
    }
    std::cout << "******************" << std::endl;
}
