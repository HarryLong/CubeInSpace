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
        const std::string fragment_shader_path(m_shader_dir.filePath(g_fragment_shader_files[ShaderType::BASE]).toStdString());
        const std::string vertex_shader_path(m_shader_dir.filePath(g_vertex_shader_files[ShaderType::BASE]).toStdString());

        m_shaders.insert( std::pair<ShaderType, ShaderProgram *>(BASE, new ShaderProgram(fragment_shader_path, vertex_shader_path, "Base Shader Program")) );
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

    // TERRAIN ELEMENTS SHADER
    {
        const std::string fragment_shader_path(m_shader_dir.filePath(g_fragment_shader_files[ShaderType::TERRAIN_ELEMENTS]).toStdString());
        const std::string vertex_shader_path(m_shader_dir.filePath(g_vertex_shader_files[ShaderType::TERRAIN_ELEMENTS]).toStdString());

        m_shaders.insert( std::pair<ShaderType, ShaderProgram *>(TERRAIN_ELEMENTS, new ShaderProgram(fragment_shader_path, vertex_shader_path, "Terrain Elements Shader Program")) );
    }

    // ORIENTATION COMPASS
    {
        const std::string fragment_shader_path(m_shader_dir.filePath(g_fragment_shader_files[ShaderType::ORIENTATION_COMPASS]).toStdString());
        const std::string vertex_shader_path(m_shader_dir.filePath(g_vertex_shader_files[ShaderType::ORIENTATION_COMPASS]).toStdString());

        m_shaders.insert( std::pair<ShaderType, ShaderProgram *>(ORIENTATION_COMPASS, new ShaderProgram(fragment_shader_path, vertex_shader_path, "Orientation Compass")) );
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

void Renderer::renderTerrain(const ViewManager * p_view, Terrain& terrain, const LightProperties & sunlight_properties)
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

    Transform transform(p_view->getProjMtx(), p_view->getViewMatrix());
    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_projection_mat)); CE();
    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_view_mat)); CE();

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

    // Material properties
    const TerrainMaterialProperties & terrain_material_properties (terrain.getMaterialProperties());
    glUniform4fv(glGetUniformLocation(prog_id, m_terrain_uniforms[MATERIAL_DIFFUSE]), 1, glm::value_ptr(terrain_material_properties.diffuse)); CE();
    glUniform4fv(glGetUniformLocation(prog_id, m_terrain_uniforms[MATERIAL_AMBIENT]), 1, glm::value_ptr(terrain_material_properties.ambient)); CE();
//    std::cout << "Material diffuse: "; Utils::print(terrain_material_properties.diffuse); std::cout << std::endl;
//    std::cout << "Material ambient: "; Utils::print(terrain_material_properties.ambient); std::cout << std::endl;

    // Overlay
    glUniform1i(glGetUniformLocation(prog_id, m_overlay_uniforms[OVERLAY_DISABLED]), m_active_terrain_overlay == OVERLAY_DISABLED); CE();
    glUniform1i(glGetUniformLocation(prog_id, m_overlay_uniforms[SLOPE_OVERLAY]), m_active_terrain_overlay == SLOPE_OVERLAY); CE();
    glUniform1i(glGetUniformLocation(prog_id, m_overlay_uniforms[ALTITUDE_OVERLAY]), m_active_terrain_overlay == ALTITUDE_OVERLAY); CE();

    // Sun Light
    glUniform4fv(glGetUniformLocation(prog_id, m_lighting_uniforms[LIGHT_DIFFUSE_COLOR]), 1, glm::value_ptr(sunlight_properties.m_diffuse_color)); CE();
    glUniform4fv(glGetUniformLocation(prog_id, m_lighting_uniforms[LIGHT_AMBIENT_COLOR]), 1, glm::value_ptr(sunlight_properties.m_ambient_color)); CE();
    glUniform4fv(glGetUniformLocation(prog_id, m_lighting_uniforms[LIGHT_POS]), 1, glm::value_ptr(sunlight_properties.getPosition())); CE();

    terrain.render();

    glUseProgram(0); // unbind
}

void Renderer::renderTerrainElements(const ViewManager * p_view, const std::vector<const Asset*> & p_assets, GLint terrain_heightmap_texture_unit)
{
    GLuint prog_id = m_shaders[TERRAIN_ELEMENTS]->getProgramID();

    glUseProgram(prog_id); CE()

    // The transformation matrices
    Transform transform(p_view->getProjMtx(), p_view->getViewMatrix());
    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_projection_mat)); CE();
    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_view_mat)); CE();

    // The heightmap texture
    GLuint heightmap_texture = glGetUniformLocation(prog_id, m_terrain_uniforms[HEIGHT_MAP_TEXTURE]); CE();
    glUniform1i(heightmap_texture, terrain_heightmap_texture_unit - GL_TEXTURE0);  CE(); // assumes texture unit 0 is bound to heightmap texture

    // Draw all the assets
    for(const Asset * asset : p_assets)
    {
        transform.m_mtw_mat = asset->getMtwMat();

        glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[MODEL_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_mtw_mat)); CE();

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

void Renderer::renderAssets(const ViewManager * p_view, const std::vector<const Asset*> & p_assets)
{
    GLuint prog_id = m_shaders[BASE]->getProgramID();

    glUseProgram(prog_id); CE()

    Transform transform(p_view->getProjMtx(), p_view->getViewMatrix());

    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_projection_mat)); CE();
    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_view_mat)); CE();

    // Draw all the assets
    for(const Asset * asset : p_assets)
    {
        transform.m_mtw_mat = asset->getMtwMat();
        transform.m_scale = asset->getScale();

        glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[MODEL_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_mtw_mat)); CE();
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

//void Renderer::renderAssets(const ViewManager * p_view, std::vector<Asset*> & p_assets)
//{
//    GLuint prog_id = m_shaders[BASE]->getProgramID();

//    glUseProgram(prog_id); CE()

//    Transform transform(p_view->getProjMtx(), p_view->getViewMatrix());

//    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_projection_mat)); CE();
//    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_view_mat)); CE();

//    // Draw all the assets
//    for(Asset * asset : p_assets)
//    {
//        Grid * grid (dynamic_cast<Grid*>(asset));

//        transform.m_mtw_mat = asset->getMtwMat();
//        transform.m_scale = asset->getScale();

//        glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[MODEL_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_mtw_mat)); CE();
//        glUniform1f(glGetUniformLocation(prog_id, m_transformation_uniforms[SCALE]), (GLfloat) transform.m_scale); CE();

//        glUniform1i(glGetUniformLocation(prog_id, m_asset_uniforms[USE_UNIFORM_COLOR]), asset->m_uniform_color );

//        if(asset->m_uniform_color) // Color data attached with verticies
//        {
//            glm::vec4 color(asset->m_color);
//            glUniform4fv(glGetUniformLocation(prog_id, m_asset_uniforms[UNIFORM_COLOR]),1, glm::value_ptr(color) );
//        }

//        grid->render();
////        glBindVertexArray(grid->m_vao_constraints); CE(); // Bind VAO

////        GLuint size((GLuint) (grid->m_verticies.size()));
////        glDrawArrays(GL_LINES, 0, 3); CE();

////        glBindVertexArray(0); CE(); // Unbind VAO

////        asset->render();
//    }

//    glUseProgram(0); // unbind
//}

//void Renderer::drawAsset(const ViewManager * p_view, SceneAsset p_asset_data, glm::vec4 color)
//{
//    GLuint prog_id = m_shaders[BASE]->getProgramID();

//    glUseProgram(prog_id); CE()

//    Transform transform(p_view->getProjMtx(), p_view->getViewMatrix(), p_asset_data.m_mtw_matrix, p_asset_data.m_scale);

//    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_projection_mat)); CE();
//    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_view_mat)); CE();
//    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[MODEL_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_mtw_mat)); CE();
//    glUniform1f(glGetUniformLocation(prog_id, m_transformation_uniforms[SCALE]), (GLfloat) transform.m_scale); CE();

//    bool uniform_color( color != no_color );
//    glUniform1i(glGetUniformLocation(prog_id, m_asset_uniforms[USE_UNIFORM_COLOR]), uniform_color );

//    if(uniform_color) // Color data attached with verticies
//    {
//        glUniform4fv(glGetUniformLocation(prog_id, m_asset_uniforms[UNIFORM_COLOR]),1, glm::value_ptr(color) );
//    }

//    // Lets Draw !
//    glBindVertexArray(p_asset_data.m_draw_data.m_vao); CE();
//    glDrawElements(p_asset_data.m_draw_data.m_draw_mode, p_asset_data.m_draw_data.m_index_buffer_size, GL_UNSIGNED_INT, (void*)(0)); CE();

//    glBindVertexArray(0); // Unbind
//    glUseProgram(0); // unbind
//}

//void Renderer::drawAsset(const ViewManager * p_view, DrawData & p_asset_data, glm::mat4x4 & p_mtw_matrix, float p_scale, glm::vec4 color)
//{
//    GLuint prog_id = m_shaders[BASE]->getProgramID();

//    glUseProgram(prog_id); CE()

//    Transform transform;
//    transform.m_projection_mat = p_view->getProjMtx();
//    //transform.view_mat = p_view->getViewMatrix();
//    transform.m_view_mat = p_view->getViewMatrix();
//    transform.m_mtw_mat = p_mtw_matrix; // For now it is the identity matrix
//    transform.m_scale = p_scale;
//    //transform.model_mat = p_view->getViewMatrix();; // For now it is the identity matrix

//    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_projection_mat)); CE();
//    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_view_mat)); CE();
//    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[MODEL_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_mtw_mat)); CE();
//    glUniform1f(glGetUniformLocation(prog_id, m_transformation_uniforms[SCALE]), (GLfloat) transform.m_scale); CE();

//    if(color != no_color) // Color data attached with verticies
//    {
//        glUniform4fv(glGetUniformLocation(prog_id, m_compass_uniforms[COLOR]),1, glm::value_ptr(arrow_colour) );

//    }

//    // Lets Draw !
//    glBindVertexArray(p_asset_data.m_vao); CE();

//    glDrawElements(GL_TRIANGLES, p_asset_data.m_index_buffer_size, GL_UNSIGNED_INT, (void*)(0)); CE();

//    glBindVertexArray(0); // Unbind
//    glUseProgram(0); // unbind
//}

//void Renderer::drawOrientationCompass(const ViewManager * p_view, DrawData & contour, DrawData & arrow, const glm::mat4x4 & mtw_matrix,
//                                      const glm::mat4x4 & north_rotation_matrix)
//{
//    std::cout << "Drawing orientation compass with vao " << contour.m_vao << std::endl;

//    GLuint prog_id = m_shaders[BASE]->getProgramID();

//    glUseProgram(prog_id); CE()

//    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(p_view->getProjMtx())); CE();
//    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(p_view->getViewMatrix() * mtw_matrix * north_rotation_matrix)); CE();

//    GLfloat base_line_width;
//    glGetFloatv(GL_LINE_WIDTH, &base_line_width);
//    glLineWidth(5);

//    static const glm::vec4 contour_colour(0,0,0,0);
//    // Draw contour
//    {
//        glUniform4fv(glGetUniformLocation(prog_id, m_asset_uniforms[USE_UNIFORM_COLOR]),1, glm::value_ptr(contour_colour) );
//        glBindVertexArray(contour.m_vao); CE();
//        glDrawElements(GL_LINE_LOOP, contour.m_index_buffer_size, GL_UNSIGNED_INT, (void*)(0)); CE();
//    }
//    static const glm::vec4 arrow_colour(1,0,0,0);
//    // Draw arrow
//    {
//        glUniform4fv(glGetUniformLocation(prog_id, m_asset_uniforms[USE_UNIFORM_COLOR]),1, glm::value_ptr(arrow_colour) );
//        glBindVertexArray(arrow.m_vao); CE();
//        glDrawElements(GL_LINES, arrow.m_index_buffer_size, GL_UNSIGNED_INT, (void*)(0)); CE();
//    }
//    glLineWidth(base_line_width);

//    glBindVertexArray(0); // Unbind
//    glUseProgram(0); // unbind
//}

//void Renderer::drawRays(const ViewManager * p_view, DrawData & p_ray_data)
//{
//    drawLines(p_view, p_ray_data);
//}

//void Renderer::drawLines(const ViewManager * p_view, Grid* p_grid)
//{
//        GLuint prog_id = m_shaders[GRID]->getProgramID(); // Use the same shader as for the grid

//        glUseProgram(prog_id); CE()

//        Transform transform(p_view->getProjMtx(), p_view->getViewMatrix(), glm::mat4x4());

//        glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_projection_mat)); CE();
//        glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_view_mat)); CE();
//        glUniform1i(glGetUniformLocation(prog_id, m_options_uniforms[DRAWING_GRID]), 1); CE();

//        // Lets Draw !
//        glBindVertexArray(p_grid->m_vao_constraints); CE();

//        glDrawArrays(GL_LINES, 0, p_grid->m_verticies.size()); CE();

//        glBindVertexArray(0); // Unbind
//        glUseProgram(0); // unbind
//}

//void Renderer::drawGrid(const ViewManager * p_view, DrawData & p_grid_data)
//{
//    drawLines(p_view, p_grid_data, true);
//}

//void Renderer::drawGrid(const ViewManager * p_view, Grid * grid)
//{
//    GLuint prog_id = m_shaders[GRID]->getProgramID(); // Use the same shader as for the grid

//    glUseProgram(prog_id); CE()

//    Transform transform(p_view->getProjMtx(), p_view->getViewMatrix());

//    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[PROJECTION_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_projection_mat)); CE();
//    glUniformMatrix4fv(glGetUniformLocation(prog_id,m_transformation_uniforms[VIEW_MAT]), 1, GL_FALSE, glm::value_ptr(transform.m_view_mat)); CE();
//    glUniform1i(glGetUniformLocation(prog_id, m_options_uniforms[DRAWING_GRID]), grid ? 1 : 0); CE();

//    // Lets Draw !
//    grid->render();
//}


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
    m_terrain_uniforms.insert(std::pair<TerrainUniforms,const char *>(TerrainUniforms::MATERIAL_DIFFUSE,"material_diffuse"));
    m_terrain_uniforms.insert(std::pair<TerrainUniforms,const char *>(TerrainUniforms::MATERIAL_AMBIENT,"material_ambient"));

    // Overlay options uniforms
    m_overlay_uniforms.insert(std::pair<TerrainOverlayUniforms,const char *>(TerrainOverlayUniforms::OVERLAY_DISABLED,"overlay.none"));
    m_overlay_uniforms.insert(std::pair<TerrainOverlayUniforms,const char *>(TerrainOverlayUniforms::SLOPE_OVERLAY,"overlay.slope"));
    m_overlay_uniforms.insert(std::pair<TerrainOverlayUniforms,const char *>(TerrainOverlayUniforms::ALTITUDE_OVERLAY,"overlay.altitude"));

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
        std::cout << "  - " << sp->getName() << " [ID: " << sp->getProgramID() << "]" << std::endl;
    }
    std::cout << "******************" << std::endl;
}
