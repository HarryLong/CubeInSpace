#ifndef TERRAIN_H
#define TERRAIN_H

#include "gl_drawable.h"
#include "include/terragen_file_manager/terragen_file.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "constants.h"
#include "asset.h"

/*******************
 * TERRAIN NORMALS *
 *******************/
class TerrainNormals : public GlDrawable
{
public:
    TerrainNormals();
    ~TerrainNormals();
    bool setTerrainDim(int width, int depth);
    GLenum getNormalsTextureUnit() const { return m_normalsTexUnit; }
//    GLuint getNormalsFBO() const { return m_fbo_normal_map; }
    bool valid() const { return m_valid; }
    void setValid( bool valid ) { m_valid = valid; }

    virtual void render() const;

protected:
    virtual bool bindBuffers();

private:
    void init();
    void delete_texture();
    void delete_fbo();

    GLfloat m_screen_quad[8] = {-1.0f, -1.0f,
                              1.0f, -1.0f,
                              1.0f, 1.0f,
                              -1.0f, 1.0f
                             };                        // for postprocessing: screen aligned quad
    GLuint m_normalsTexture;
    GLuint m_normalsTexUnit;
    GLuint m_fbo_normal_map; // normal map FBO

    int m_width, m_depth;

    bool m_valid;
};

/*********************
 * TERRAIN RECTANGLE *
 *********************/
class TerrainRect : public Asset
{
public:
    TerrainRect(glm::vec3 min, glm::vec3 max, int terrain_width, int terrain_depth);
    ~TerrainRect();
    virtual void render() const;

protected:
    virtual bool bindBuffers();
};

struct SphereAccelerationStructure
{
public:
    struct Sphere {
    public:
        Sphere(glm::vec3 center, float radius) : center(center), radius(radius) {}
        glm::vec3 center;
        float radius;
    };

    SphereAccelerationStructure() : step_size(SPHERE_ACCELERATION_STRUCTURE_STEP_SIZE), n_spheres_x(0), n_spheres_z(0) {}

    inline void clear() { m_spheres.clear(); }

    const int step_size;
    int n_spheres_x;
    int n_spheres_z;

    // Spheres ordered as x,y
    std::vector<std::vector<Sphere> > m_spheres;
};

struct TerrainMaterialProperties{
    const glm::vec4 diffuse = glm::vec4(0.7f, 0.6f, 0.5f, 1.0f); // colour of terrain
    const glm::vec4 specular = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    const glm::vec4 ambient = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
};

class QProgressDialog;

class Terrain : public GlDrawable
{
public:
    Terrain();
    ~Terrain();
    virtual void render() const;

    const std::vector<const Asset*> getTerrainElements();

    bool setTerrain(TerragenFile parsed_terrangen_file);
    float getMaxHeight() const { return m_terragen_file.m_header_data.max_height; }
    float getBaseHeight() const { return m_terragen_file.m_header_data.base_height; }
    GLenum getHeightMapTextureUnit() const { return m_htmapTexUnit; }
    GLenum getShadeTextureUnit() const { return m_shadeTexUnit; }

    const TerrainNormals & getNormals() const {return m_terrain_normals; }
    int getWidth() const {return m_terragen_file.m_header_data.width; }
    int getDepth() const {return m_terragen_file.m_header_data.depth; }
    void normalsRecalculated() { m_terrain_normals.setValid(true); }
    bool traceRay(glm::vec3 start_point, glm::vec3 direction, glm::vec3 & intersection_point);

    void incrementHeights(const std::vector<glm::vec3> & points, int increment);
    void addTerrainRect(glm::vec3 min, glm::vec3 max);
    void clearTerrainElements();
    const TerrainMaterialProperties & getMaterialProperties() {return m_material_properties; }

    glm::vec2 getCenter();

    const SphereAccelerationStructure& getSphereAccelerationStructure() { return m_sphere_acceleration_structure; }
    bool isShadeOverlayReady();
    void refreshShadingTexture(QProgressDialog * progress_dialog);

signals:
    void terrainChanged(int width, int depth, int base_height, int max_height);

public slots:
    void setSunPosition(float sun_pos_x, float sun_pos_y, float sun_pos_z);

protected:
    virtual bool bindBuffers();

private:
    void init();
    void prepare_terrain_geometry();
    void build_sphere_acceleration_structure();
    bool ray_intersect(const glm::vec3 & start, const glm::vec3 & direction, glm::vec3 & intersection_point);
    void refresh_heightmap_texture(TerragenFile & parsed_terrangen_file);
    void clear_terrain_rectangles();
    void delete_heightmap_texture();
    void delete_shade_texture();

    // Textures
    GLuint m_heightmapTexture; // Heightmap texture
    GLenum m_htmapTexUnit; // Heightmap texture unit

    GLuint m_shadeTexture; // Shade texture
    GLenum m_shadeTexUnit; // Shade texture unit
    bool m_shade_overlay_ready;
    glm::vec3 m_sun_position;

    TerragenFile m_terragen_file;
    TerrainNormals m_terrain_normals;

    SphereAccelerationStructure m_sphere_acceleration_structure;

    std::vector<TerrainRect*> m_terrain_rectangles;

    TerrainMaterialProperties m_material_properties;

    glm::vec2 m_center;
};


#endif // TERRAIN_H
