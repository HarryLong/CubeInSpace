#ifndef RENDERER_H
#define RENDERER_H

#include "shader_program.h"
#include <string>
#include <map>
#include <vector>
#include "view_manager.h"
#include "gl_drawable.h"
#include <QDir>
#include "terrain.h"
#include "constants.h"

/* SHADERS */
enum ShaderType {
    ASSET = 0,
    GRID,
    TERRAIN,
    NORMALS
};

// ensure to respect order defines in shader enum
const QString g_vertex_shader_files[] = {
    "base.vert",
    "grid.vert",
    "terrain.vert",
    "normals_generator.vert"
};

// ensure to respect order defines in shader enum
const QString g_fragment_shader_files[] = {
    "base.frag",
    "grid.frag",
    "terrain.frag",
    "normals_generator.frag"
};

enum TransformationUniforms{
    PROJECTION_MAT, // Model-view Matrix
    VIEW_MAT, // Model-view-projection matrix
    MODEL_MAT,
    SCALE
};

enum TerrainUniforms
{
    HEIGHT_MAP_TEXTURE,
    NORMALS_TEXTURE,
    MAX_HEIGHT,
    BASE_HEIGHT,
    TERRAIN_SIZE,
};

enum TerrainOverlayUniforms{
    OVERLAY_DISABLED,
    SLOPE_OVERLAY,
    ALTITUDE_OVERLAY,
};

enum LightingUniforms{
    MATERIAL_DIFFUSE,
    MATERIAL_AMBIENT,
    LIGHT_DIFFUSE_COLOR,
    LIGHT_AMBIENT_COLOR,
    LIGHT_POS,
};

const glm::vec4 default_grid_color(1,1,1,.1);

struct Transform{
    glm::mat4x4 projection_mat;
    glm::mat4x4 view_mat;
    glm::mat4x4 model_mat;
};

struct LightProperties{
    const int n_light_pos = 4;
    const glm::vec4 terrain_material_diffuse = glm::vec4(0.7f, 0.6f, 0.5f, 1.0f); // colour of terrain
    const glm::vec4 terrain_material_specular = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    const glm::vec4 terrain_material_ambient = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
    const glm::vec4 light_color_diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // colour of light
    const glm::vec4 light_color_specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    const glm::vec4 light_color_ambient = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
    const float shiny_spec = 5.0f; // specular power
};

class Renderer {
public:
    Renderer(const std::string& m_shader_dir);
    ~Renderer();
    void drawTerrain(const ViewManager * p_view, Terrain& terrain);
    void drawAsset(const ViewManager * p_view, DrawData & p_asset_data, glm::mat4x4 & p_mtw_matrix, float p_scale);
    void drawGrid(const ViewManager * p_view, DrawData & p_grid_data);
    void drawRays(const ViewManager * p_view, DrawData & p_ray_data);

    void printShaders();
    void setOverlay(TerrainOverlayUniforms overlay);

private:
    void initUniforms();
    void initShaders();
    std::map<ShaderType, ShaderProgram *> m_shaders;
    QDir m_shader_dir;
    LightProperties m_lighting_properties;

    // Uniform holders
    std::map<TransformationUniforms,const char *> m_transformation_uniforms;
    std::map<TerrainUniforms,const char *> m_terrain_uniforms;
    std::map<TerrainOverlayUniforms,const char *> m_overlay_uniforms;
    std::map<LightingUniforms,const char *> m_lighting_uniforms;

    TerrainOverlayUniforms m_active_terrain_overlay;
};

#endif // RENDERER_H
