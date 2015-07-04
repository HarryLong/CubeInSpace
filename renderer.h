#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <map>
#include <vector>
#include <QDir>
#include "terrain.h"
#include "constants.h"
#include "light_properties.h"
#include <QMatrix>
#include "grid.h"
#include "transform.h"

class QOpenGLShaderProgram;
class TerrainWater;
class WaterFluxGeneratorShader;

/* SHADERS */
enum ShaderType {
    BASE = 0,
    TERRAIN,
    TERRAIN_ELEMENTS,
    NORMALS,
    WATER_FLUX
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
    MAX_HEIGHT,
    BASE_HEIGHT,
    TERRAIN_SIZE,
    TERRAIN_MATERIAL_DIFFUSE,
    TERRAIN_MATERIAL_AMBIENT
};

enum WaterUniforms
{
    WATER_MATERIAL_DIFFUSE,
    WATER_MATERIAL_AMBIENT
};

enum TextureUniforms
{
    TERRAIN_HEIGHT_MAP_TEXTURE,
    WATER_HEIGHT_MAP_TEXTURE,
    NORMALS_TEXTURE,
    SHADE_TEXTURE,
    MIN_TEMPERATURE_TEXTURE,
    MAX_TEMPERATURE_TEXTURE,
    MIN_DAILY_ILLUMINATION_TEXTURE,
    MAX_DAILY_ILLUMINATION_TEXTURE
};

enum TerrainOverlayUniforms{
    OVERLAY_DISABLED,
    SLOPE_OVERLAY,
    ALTITUDE_OVERLAY,
    SHADE_OVERLAY,
    TEMPERATURE_MIN,
    TEMPERATURE_MAX,
    DAILY_ILLUMINATION_MIN,
    DAILY_ILLUMINATION_MAX
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

//class Transform{
//public:
//    Transform(glm::mat4x4 projection_mat, glm::mat4x4 view_mat, glm::mat4x4 mtw_mat = glm::mat4x4(), float scale = 1.0f) :
//        m_projection_mat(projection_mat), m_mtw_mat(mtw_mat), m_view_mat(view_mat), m_scale(scale) {}

//    glm::mat4x4 m_projection_mat;
//    glm::mat4x4 m_view_mat;
//    glm::mat4x4 m_mtw_mat;
//    float m_scale;
//};

class Renderer {
public:
    Renderer();
    ~Renderer();
    void init();

    void renderTerrain(QOpenGLShaderProgram * shader, const Transform & p_transforms, Terrain * terrain, const LightProperties & sunlight_properties);
    void renderTerrainElements(QOpenGLShaderProgram * shader, const Transform & p_transforms, const std::vector<Asset*> & p_assets, Terrain * terrain);
    void renderAssets(QOpenGLShaderProgram * shader, const Transform & p_transforms, const std::vector<Asset*> & p_assets);
    void calculateNormals(QOpenGLShaderProgram * shader, Terrain * terrain);

    void tmp_function1(WaterFluxGeneratorShader * shader, Terrain * terrain);

private:
    void init_uniforms();
    void init_shaders();
    void append_shader(const ShaderType & shader_type, const QString & description);
    void reset_overlays(QOpenGLShaderProgram * shader);
//    std::map<ShaderType, ShaderProgram *> m_shaders;

    // Uniform holders
    std::map<TransformationUniforms,const char *> m_transformation_uniforms;
    std::map<TerrainUniforms,const char *> m_terrain_uniforms;
    std::map<TerrainOverlayUniforms,const char *> m_overlay_uniforms;
    std::map<LightingUniforms,const char *> m_lighting_uniforms;
    std::map<OptionUniforms,const char *> m_options_uniforms;
    std::map<AssetUniforms,const char *> m_asset_uniforms;
    std::map<TextureUniforms,const char *> m_texture_uniforms;
    std::map<WaterUniforms,const char *> m_water_uniforms;
};

#endif // RENDERER_H
