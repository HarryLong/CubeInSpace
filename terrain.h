#ifndef TERRAIN_H
#define TERRAIN_H

#include "gl_drawable.h"
#include "include/terragen_file_manager/terragen_file.h"
#include <glm/common.hpp>
#include "constants.h"
#include "asset.h"
#include "controllers.h"
#include "acceleration_structure.h"
#include "terrain_normals.h"
#include "terrain_shade.h"
#include "terrain_temperature.h"
#include "terrain_daily_illimination.h"
#include <QOpenGLTexture>

/***********************
 * MATERIAL PROPERTIES *
 ***********************/
struct TerrainMaterialProperties{
    static const glm::vec4 diffuse;
    static const glm::vec4 specular;
    static const glm::vec4 ambient;
};

/****************
 * BASE TERRAIN *
 ****************/
class BaseTerrain : public TerragenFile
{
public:
    BaseTerrain();
    virtual ~BaseTerrain();
};

/********************
 * DRAWABLE TERRAIN *
 ********************/
class DrawableTerrain : public GlDrawable, public TextureElement<GLfloat>
{
public:
    DrawableTerrain();
    ~DrawableTerrain();

    virtual void render();

    void setMetersPerTerrainUnit(int meters_per_tu);

    bool prepareTerrainGeometry(const TerragenFile & terragen_file);
    void refreshHeightmapTexture(const TerragenFile & terragen_file);

protected:
    virtual void initGL();

private:
    void delete_heightmap_texture();
};

/***********
 * TERRAIN *
 ***********/
class TemperatureEditDialog;
class Actions;
class GLSelectionRect;
class Terrain : public QObject
{
    Q_OBJECT    
public:

    Terrain(TerrainControllers & terrain_controllers, Actions * overlay_actions);
    ~Terrain();

    Asset* getSelectionRect();
    void setTerrain(TerragenFile parsed_terrangen_file);
    float getMaxHeight(bool scaled = false) const;
    float getBaseHeight(bool scaled = false) const;
    int getWidth(bool scaled = false) const;
    int getDepth(bool scaled = false) const;
    bool traceRay(glm::vec3 start_point, glm::vec3 direction, glm::vec3 & intersection_point);
    void setSelectionRectangle(glm::vec3 min, glm::vec3 max);
    const TerrainMaterialProperties & getMaterialProperties();
    const SphericalAccelerationStructure<Hierarchical>& getSphereAccelerationStructure();
    float getScale();
    void loadBaseTerrain();
    void getTerrainDimensions(int & width, int & depth, int & base_height, int & max_height );
    void render();
    // Terrain normals functions
    void renderNormals();
    bool normalsValid();
    float getAltitude(const glm::vec3 & point);
    float getSlope(const glm::vec3 & point);
    bool isShaded(const glm::vec3 & point, bool & shaded);
    bool getTemperatures(const glm::vec3 & point, float & min, float & max);
    bool getDailyIlluminations(const glm::vec3 & point, int & min, int & max);

    // Heightmap functions
//    GLenum getHeightMapTextureUnit() const;
//    GLenum getShadeTextureUnit() const;
//    GLenum getNormalsTextureUnit() const;
//    GLenum getMinTemperatureTextureUnit() const;
//    GLenum getMaxTemperatureTextureUnit() const;
//    GLenum getMinDailyIlluminationTextureUnit() const;
//    GLenum getMaxDailyIlluminationTextureUnit() const;
//    GLint getHeightmapTexture() const;

    // Overlays
    bool overlayNone() const;
    bool overlaySlope() const;
    bool overlayAltitude() const;
    bool overlayShade() const;
    bool overlayMinTemp() const;
    bool overlayMaxTemp() const;
    bool overlayMinDailyIllumination() const;
    bool overlayMaxDailyIllumination() const;

    DrawableTerrain & getDrawableTerrain();
    TerrainShade * getShade();
    TerrainNormals * getNormals();
    TerrainTemperature * getMinTemp();
    TerrainTemperature * getMaxTemp();
    TerrainDailyIllumination * getMinDailyIllumination();
    TerrainDailyIllumination * getMaxDailyIllumination();

signals:
    void terrainDimensionsChanged(int width, int depth, int base_height, int max_height);
    void activeOverlayChanged();
    void processing(QString info);
    void intermediate_processing_update(int progress);
    void processing_complete();
    void trigger_daily_illumination_refresh();

public slots:
    void setSunPosition(float sun_pos_x, float sun_pos_y, float sun_pos_z);
    void refreshShade(QString display_message = "Calculating shade...");
    void invalidateIllumination(); // Called when latitude changes
    void setMinIlluminationData(GLubyte * data);
    void setMaxIlluminationData(GLubyte * data);
    void refreshTemperature(float min_temp_at_zero, float min_lapse_rate, float max_temp_at_zero, float max_lapse_rate);

private slots:
    void recalculate_scale();
    void invalidate_shade();
    void selected_overlay_changed();
    void invalidate_temp();
    void refresh_illumination();
    void reset_overlay();

private:
    bool ray_intersect(const glm::vec3 & start, const glm::vec3 & direction, glm::vec3 & intersection_point, bool search_closest = true);
    void establish_connections();
    float get_altitude(const glm::vec2 & point);
    float get_slope(const glm::vec2 & point);
    bool get_temperatures(const glm::vec2 & point, float & min, float & max);
    bool is_shaded(const glm::vec2 & point, bool & shaded);
    bool get_daily_illuminations(const glm::vec2 & point, int & min, int & max);
    void clear_selection_rectangle();

    TerrainControllers m_terrain_controllers;
    glm::vec3 m_sun_position;
    TerragenFile m_terragen_file;
    SphericalAccelerationStructure<Hierarchical> m_sphere_acceleration_structure;
    GLSelectionRect * m_selection_rectangle;
    TerrainMaterialProperties m_material_properties;
    float m_scale;
    DrawableTerrain m_drawable_terrain;
    TerrainShade * m_terrain_shade;
    TerrainNormals * m_terrain_normals;
    TerrainTemperature * m_terrain_min_temp;
    TerrainTemperature * m_terrain_max_temp;
    TerrainDailyIllumination * m_terrain_min_daily_illumination;
    TerrainDailyIllumination * m_terrain_max_daily_illumination;
    Actions * m_overlay_actions;
};

#endif // TERRAIN_H
