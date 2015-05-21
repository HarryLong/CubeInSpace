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
#include "light_properties.h"

/* SHADERS */
enum ShaderType {
    ASSET = 0,
    GRID,
    TERRAIN,
    TERRAIN_ELEMENTS,
    ORIENTATION_COMPASS,
    NORMALS
};

// ensure to respect order defines in shader enum
const QString g_vertex_shader_files[] = {
    "base.vert",
    "grid.vert",
    "terrain.vert",
    "terrain_elements.vert",
    "orientation.vert",
    "normals_generator.vert"
};

// ensure to respect order defines in shader enum
const QString g_fragment_shader_files[] = {
    "base.frag",
    "grid.frag",
    "terrain.frag",
    "terrain_elements.frag",
    "orientation.frag",
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
    MATERIAL_DIFFUSE,
    MATERIAL_AMBIENT,
};

enum TerrainOverlayUniforms{
    OVERLAY_DISABLED,
    SLOPE_OVERLAY,
    ALTITUDE_OVERLAY,
};

enum LightingUniforms{
    LIGHT_DIFFUSE_COLOR,
    LIGHT_AMBIENT_COLOR,
    LIGHT_POS,
};

enum CompassUniforms{
    COLOR,
};

enum OptionUniforms{
    DRAWING_GRID,
};

const glm::vec4 default_grid_color(1,1,1,.1);

struct Transform{
    glm::mat4x4 projection_mat;
    glm::mat4x4 view_mat;
    glm::mat4x4 model_mat;
};

class Renderer {
public:
    Renderer(const std::string& m_shader_dir);
    ~Renderer();
    void drawTerrain(const ViewManager * p_view, Terrain& terrain, const LightProperties & sunlight_properties);
    void drawTerrainElements(const ViewManager * p_view, const std::vector<DrawData> & terrain_elements, GLint terrain_heightmap_texture_unit);
    void drawAsset(const ViewManager * p_view, DrawData & p_asset_data, glm::mat4x4 & p_mtw_matrix, float p_scale);
    void drawGrid(const ViewManager * p_view, DrawData & p_grid_data);
    void drawRays(const ViewManager * p_view, DrawData & p_ray_data);
    void drawOrientationCompass(const ViewManager * p_view, DrawData & contour, DrawData & arrow, const glm::mat4x4 & compass_translation_matrix,
                                const glm::mat4x4 & north_rotation_matrix);
    void drawLines(const ViewManager * p_view, DrawData & p_ray_data, bool grid = false);
    void printShaders();
    void setOverlay(TerrainOverlayUniforms overlay);

private:
    void initUniforms();
    void initShaders();
    std::map<ShaderType, ShaderProgram *> m_shaders;
    QDir m_shader_dir;

    // Uniform holders
    std::map<TransformationUniforms,const char *> m_transformation_uniforms;
    std::map<TerrainUniforms,const char *> m_terrain_uniforms;
    std::map<TerrainOverlayUniforms,const char *> m_overlay_uniforms;
    std::map<LightingUniforms,const char *> m_lighting_uniforms;
    std::map<OptionUniforms,const char *> m_options_uniforms;
    std::map<CompassUniforms,const char *> m_compass_uniforms;

    TerrainOverlayUniforms m_active_terrain_overlay;
};

#endif // RENDERER_H
