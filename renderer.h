#ifndef RENDERER_H
#define RENDERER_H

#include "shader_program.h"
#include <string>
#include <map>
#include <vector>
#include "view_manager.h"
#include <QDir>
#include "terrain.h"
#include "constants.h"
#include "light_properties.h"

#include "grid.h"

/* SHADERS */
enum ShaderType {
    BASE = 0,
    TERRAIN,
    TERRAIN_ELEMENTS,
    NORMALS
};

// ensure to respect order defines in shader enum
const QString g_vertex_shader_files[] = {
    ":/base.vert",
    ":/terrain.vert",
    ":/terrain_elements.vert",
    ":/normals_generator.vert"
};

// ensure to respect order defines in shader enum
const QString g_fragment_shader_files[] = {
    ":/base.frag",
    ":/terrain.frag",
    ":/terrain_elements.frag",
    ":/normals_generator.frag"
};

enum AssetUniforms {
    UNIFORM_COLOR,
    USE_UNIFORM_COLOR
};

enum TransformationUniforms{
    PROJECTION_MAT,
    VIEW_MAT,
    MTW_MAT,
    SCALE
};

enum TerrainUniforms
{
    HEIGHT_MAP_TEXTURE,
    NORMALS_TEXTURE,
    SHADE_TEXTURE,
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
    SHADE_OVERLAY
};

enum LightingUniforms{
    LIGHT_DIFFUSE_COLOR,
    LIGHT_AMBIENT_COLOR,
    LIGHT_POS,
};

enum OptionUniforms{
    DRAWING_GRID,
};

const glm::vec4 default_grid_color(1,1,1,.1);

class Transform{
public:
    Transform(glm::mat4x4 projection_mat, glm::mat4x4 view_mat, glm::mat4x4 mtw_mat = glm::mat4x4(), float scale = 1.0f) :
        m_projection_mat(projection_mat), m_mtw_mat(mtw_mat), m_view_mat(view_mat), m_scale(scale) {}

    glm::mat4x4 m_projection_mat;
    glm::mat4x4 m_view_mat;
    glm::mat4x4 m_mtw_mat;
    float m_scale;
};

class Renderer {
public:
    Renderer();
    ~Renderer();
    void init();

    void renderTerrain(const ViewManager & p_view, Terrain& terrain, const LightProperties & sunlight_properties);
    void renderTerrainElements(const ViewManager & p_view, const std::vector<const Asset*> & p_assets, GLint terrain_heightmap_texture_unit);
    void renderAssets(const ViewManager & p_view, const std::vector<const Asset*> & p_assets);

    void printShaders();
    void setOverlay(TerrainOverlayUniforms overlay);

private:
    void init_uniforms();
    void init_shaders();
    void append_shader(const ShaderType & shader_type, const QString & description);

    std::map<ShaderType, ShaderProgram *> m_shaders;

    // Uniform holders
    std::map<TransformationUniforms,const char *> m_transformation_uniforms;
    std::map<TerrainUniforms,const char *> m_terrain_uniforms;
    std::map<TerrainOverlayUniforms,const char *> m_overlay_uniforms;
    std::map<LightingUniforms,const char *> m_lighting_uniforms;
    std::map<OptionUniforms,const char *> m_options_uniforms;
    std::map<AssetUniforms,const char *> m_asset_uniforms;

    TerrainOverlayUniforms m_active_terrain_overlay;
};

#endif // RENDERER_H
