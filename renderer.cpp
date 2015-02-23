#include "renderer.h"
#include "glheader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "view_manager.h"
#include "utils/utils.h"

Renderer::Renderer(const std::string& p_shader_dir) : m_shader_dir(QString(p_shader_dir.c_str())), m_active_terrain_overlay(OVERLAY_DISABLED)
{
    initUniforms();
    initShaders();
}

Renderer::~Renderer()
{
    for(auto it = m_shaders.begin(); it != m_shaders.end(); it++)
    {
        delete it->second;
    }
}

void Renderer::initShaders()
{
    // BASE SHADER
    {
        const std::string fragment_shader_path(m_shader_dir.filePath(g_fragment_shader_files[ShaderType::ASSET]).toStdString());
        const std::string vertex_shader_path(m_shader_dir.filePath(g_vertex_shader_files[ShaderType::ASSET]).toStdString());

        m_shaders.insert( std::pair<ShaderType, ShaderProgram *>(ASSET, new ShaderProgram(fragment_shader_path, vertex_shader_path, "Base Shader Program")) );
    }

    // GRID SHADER
    {
        const std::string fragment_shader_path(m_shader_dir.filePath(g_fragment_shader_files[ShaderType::GRID]).toStdString());
        const std::string vertex_shader_path(m_shader_dir.filePath(g_vertex_shader_files[ShaderType::GRID]).toStdString());

        m_shaders.insert( std::pair<ShaderType, ShaderProgram *>(GRID, new ShaderProgram(fragment_shader_path, vertex_shader_path, "Grid Shader Program")) );
    }

    // TERRAIN SHADER
    {
        const std::string fragment_shader_path(m_shader_dir.filePath(g_fragment_shader_files[ShaderType::TERRAIN]).toStdString());
        const std::string vertex_shader_path(m_shader_dir.filePath(g_vertex_shader_files[ShaderType::TERRAIN]).toStdString());

        m_shaders.insert( std::pair<ShaderType, ShaderProgram *>(TERRAIN, new ShaderProgram(fragment_shader_path, vertex_shader_path, "Terrain Shader Program")) );
    }

    // NORMALS SHADER
    {
        const std::string fragment_shader_path(m_shader_dir.filePath(g_fragment_shader_files[ShaderType::NORMALS]).toStdString());
        const std::string vertex_shader_path(m_shader_dir.filePath(g_vertex_shader_files[ShaderType::NORMALS]).toStdString());

        m_shaders.insert( std::pair<ShaderType, ShaderProgram *>(NORMALS, new ShaderProgram(fragment_shader_path, vertex_shader_path, "Normal Generator Shader Program")) );
    }

    // Compile and link the shaders
    for(auto it = m_shaders.begin(); it != m_shaders.end(); it++)
    {
        ShaderProgram * sp = it-> second;
        if(!sp->compileAndLink())
            std::cerr << "Failed to compile shader: " << sp->getName();
    }
}

void Renderer::drawTerrain(const ViewManager * p_view, Terrain& terrain)
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

        // Render to our framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, terrain.getNormals().getNormalsFBO()); CE();

        // set shader program to normal map gen
        glBindVertexArray(terrain.getNormals().getDrawData().m_vao); CE();

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);  CE();

        // unbind everthing
        glBindFramebuffer(GL_FRAMEBUFFER, 0);  CE();
        glBindVertexArray(0);  CE();
        glUseProgram(0);  CE();

        // reset viewport
        glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);

        terrain.normalsRecalculated();
    }

    GLuint prog_id = m_shaders[TERRAIN]->getProgramID();

    glUseProgram(prog_id); CE()

    Transform transform;
    transform.projection_mat = p_view->getProjMtx();
    transform.view_mat = p_view->getViewMatrix();

    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(transform.projection_mat)); CE();
    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.view_mat)); CE();

    // Heightmap
    GLuint heightmap_texture = glGetUniformLocation(prog_id, m_terrain_uniforms[HEIGHT_MAP_TEXTURE]); CE();
    glUniform1i(heightmap_texture, (GLint)(terrain.getHeightMapTextureUnit() - GL_TEXTURE0));  CE(); // assumes texture unit 0 is bound to heightmap texture

    // Normals
    GLuint normals_texture = glGetUniformLocation(prog_id, m_terrain_uniforms[NORMALS_TEXTURE]); CE();
    glUniform1i(normals_texture, (GLint)(terrain.getNormals().getNormalsTextureUnit() - GL_TEXTURE0));  CE(); // assumes texture unit 0 is bound to heightmap texture

    // Heightmap maximum height
    glUniform1f(glGetUniformLocation(prog_id, m_terrain_uniforms[MAX_HEIGHT]), (GLfloat) terrain.getMaxHeight() ); CE();

    // Heightmap base height
    glUniform1f(glGetUniformLocation(prog_id, m_terrain_uniforms[BASE_HEIGHT]), (GLfloat) terrain.getBaseHeight()); CE();

    // Overlay
    glUniform1i(glGetUniformLocation(prog_id, m_overlay_uniforms[OVERLAY_DISABLED]), m_active_terrain_overlay == OVERLAY_DISABLED); CE();
    glUniform1i(glGetUniformLocation(prog_id, m_overlay_uniforms[SLOPE_OVERLAY]), m_active_terrain_overlay == SLOPE_OVERLAY); CE();
    glUniform1i(glGetUniformLocation(prog_id, m_overlay_uniforms[ALTITUDE_OVERLAY]), m_active_terrain_overlay == ALTITUDE_OVERLAY); CE();

    // Lighting
    glUniform4fv(glGetUniformLocation(prog_id, m_lighting_uniforms[MATERIAL_DIFFUSE]), 1, glm::value_ptr(m_lighting_properties.terrain_material_diffuse)); CE();
    glUniform4fv(glGetUniformLocation(prog_id, m_lighting_uniforms[MATERIAL_AMBIENT]), 1, glm::value_ptr(m_lighting_properties.terrain_material_ambient)); CE();
    glUniform4fv(glGetUniformLocation(prog_id, m_lighting_uniforms[LIGHT_DIFFUSE_COLOR]), 1, glm::value_ptr(m_lighting_properties.light_color_diffuse)); CE();
    glUniform4fv(glGetUniformLocation(prog_id, m_lighting_uniforms[LIGHT_AMBIENT_COLOR]), 1, glm::value_ptr(m_lighting_properties.light_color_ambient)); CE();

    // Calculate the light position
//    glm::vec4 light_pos2 = glm::vec4((terrain.getWidth() * terrain.getDynamicScale()) / 2, terrain.getMaxHeight() * 10,
//                                      -terrain.getWidth() * terrain.getDynamicScale(), 1.0f);

    glm::vec4 light_pos2 = glm::vec4(500.0f, terrain.getMaxHeight() * 10.0f, -500.0f, 1.0f);
    glUniform4fv(glGetUniformLocation(prog_id, m_lighting_uniforms[LIGHT_POS]), 1, glm::value_ptr(light_pos2)); CE();

    // Lets Draw !
    glBindVertexArray(terrain.getDrawData().m_vao); CE();

    glDrawElements(GL_TRIANGLE_STRIP, terrain.getDrawData().m_index_buffer_size, GL_UNSIGNED_INT, (void*)(0)); CE();

    glBindVertexArray(0); // Unbind
    glUseProgram(0); // unbind
}

void Renderer::drawAsset(const ViewManager * p_view, DrawData & p_asset_data, glm::mat4x4 & p_mtw_matrix, float p_scale)
{
    GLuint prog_id = m_shaders[ASSET]->getProgramID();

    glUseProgram(prog_id); CE()

    Transform transform;
    transform.projection_mat = p_view->getProjMtx();
    //transform.view_mat = p_view->getViewMatrix();
    transform.view_mat = p_view->getViewMatrix();
    transform.model_mat = p_mtw_matrix; // For now it is the identity matrix
    //transform.model_mat = p_view->getViewMatrix();; // For now it is the identity matrix

    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(transform.projection_mat)); CE();
    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.view_mat)); CE();
    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[MODEL_MAT]), 1, GL_FALSE, glm::value_ptr(transform.model_mat)); CE();
    glUniform1f(glGetUniformLocation(prog_id, m_transformation_uniforms[SCALE]), (GLfloat) p_scale); CE();

    // Lets Draw !
    glBindVertexArray(p_asset_data.m_vao); CE();

    glDrawElements(GL_TRIANGLES, p_asset_data.m_index_buffer_size, GL_UNSIGNED_INT, (void*)(0)); CE();

    glBindVertexArray(0); // Unbind
    glUseProgram(0); // unbind
}

void Renderer::drawRays(const ViewManager * p_view, DrawData & p_ray_data)
{
    if(p_ray_data.m_vertex_buffer_size> 0)
    {
        GLuint prog_id = m_shaders[GRID]->getProgramID(); // Use the same shader as for the grid

        glUseProgram(prog_id); CE()

        Transform transform;
        transform.projection_mat = p_view->getProjMtx();
        transform.view_mat = p_view->getViewMatrix();

        glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(transform.projection_mat)); CE();
        glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.view_mat)); CE();

        // Lets Draw !
        glBindVertexArray(p_ray_data.m_vao); CE();

        glDrawArrays(GL_LINES, 0, p_ray_data.m_vertex_buffer_size); CE();

        glBindVertexArray(0); // Unbind
        glUseProgram(0); // unbind
    }
}

void Renderer::drawGrid(const ViewManager * p_view, DrawData & p_grid_data)
{
    GLuint prog_id = m_shaders[GRID]->getProgramID();

    glUseProgram(prog_id); CE()

    Transform transform;
    transform.projection_mat = p_view->getProjMtx();
    transform.view_mat = p_view->getViewMatrix();

    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(transform.projection_mat)); CE();
    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.view_mat)); CE();

    // Lets Draw !
    glBindVertexArray(p_grid_data.m_vao); CE();

    glDrawElements(GL_LINES, p_grid_data.m_index_buffer_size, GL_UNSIGNED_INT, (void*)(0)); CE();

    glBindVertexArray(0); // Unbind
    glUseProgram(0); // unbind
}

void Renderer::initUniforms()
{
    // Transformation uniforms
    m_transformation_uniforms.insert(std::pair<TransformationUniforms,const char *>(TransformationUniforms::PROJECTION_MAT,"transform.projMat"));
    m_transformation_uniforms.insert(std::pair<TransformationUniforms,const char *>(TransformationUniforms::VIEW_MAT,"transform.viewMat"));
    m_transformation_uniforms.insert(std::pair<TransformationUniforms,const char *>(TransformationUniforms::MODEL_MAT,"transform.modelMat"));
    m_transformation_uniforms.insert(std::pair<TransformationUniforms,const char *>(TransformationUniforms::SCALE,"transform.scale"));

    // Terrain uniforms
    m_terrain_uniforms.insert(std::pair<TerrainUniforms,const char *>(TerrainUniforms::HEIGHT_MAP_TEXTURE,"height_map_texture"));
    m_terrain_uniforms.insert(std::pair<TerrainUniforms,const char *>(TerrainUniforms::NORMALS_TEXTURE,"normals_texture"));
    m_terrain_uniforms.insert(std::pair<TerrainUniforms,const char *>(TerrainUniforms::MAX_HEIGHT,"max_height"));
    m_terrain_uniforms.insert(std::pair<TerrainUniforms,const char *>(TerrainUniforms::BASE_HEIGHT,"base_height"));
    m_terrain_uniforms.insert(std::pair<TerrainUniforms,const char *>(TerrainUniforms::TERRAIN_SIZE,"terrain_size"));

    // Overlay options uniforms
    m_overlay_uniforms.insert(std::pair<TerrainOverlayUniforms,const char *>(TerrainOverlayUniforms::OVERLAY_DISABLED,"overlay.none"));
    m_overlay_uniforms.insert(std::pair<TerrainOverlayUniforms,const char *>(TerrainOverlayUniforms::SLOPE_OVERLAY,"overlay.slope"));
    m_overlay_uniforms.insert(std::pair<TerrainOverlayUniforms,const char *>(TerrainOverlayUniforms::ALTITUDE_OVERLAY,"overlay.altitude"));

    // Lighting uniforms
    m_lighting_uniforms.insert(std::pair<LightingUniforms,const char *>(LightingUniforms::MATERIAL_DIFFUSE,"material_diffuse"));
    m_lighting_uniforms.insert(std::pair<LightingUniforms,const char *>(LightingUniforms::MATERIAL_AMBIENT,"material_ambient"));
    m_lighting_uniforms.insert(std::pair<LightingUniforms,const char *>(LightingUniforms::LIGHT_POS,"light_position"));
    m_lighting_uniforms.insert(std::pair<LightingUniforms,const char *>(LightingUniforms::LIGHT_DIFFUSE_COLOR,"light_diffuse_color"));
    m_lighting_uniforms.insert(std::pair<LightingUniforms,const char *>(LightingUniforms::LIGHT_AMBIENT_COLOR,"light_ambient_color"));
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
        std::cout << "  - " << sp->getName() << " [ID: " << sp->getProgramID() << "]" << std::endl;
    }
    std::cout << "******************" << std::endl;
}
