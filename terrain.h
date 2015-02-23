#ifndef TERRAIN_H
#define TERRAIN_H

#include "gl_drawable.h"
#include "terragen/terragen_file_spec.h"
#include "glm/vec3.hpp"
#include "constants.h"

class TerrainNormals : public GlDrawable
{
public:
    TerrainNormals();
    ~TerrainNormals();
    bool bindBuffers();
    bool setTerrainDim(int width, int depth);
    GLenum getNormalsTextureUnit() const { return m_normalsTexUnit; }
    GLuint getNormalsFBO() const { return m_fbo_normal_map; }
    bool valid() const { return m_valid; }
    void setValid( bool valid ) { m_valid = valid; }

private:
    void init();
    virtual void delete_buffers();

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

struct Sphere {
public:
    Sphere(glm::vec3 center, float radius) : center(center), radius(radius) {}
    glm::vec3 center;
    float radius;
};

struct SphereAccelerationStructure
{
public:
    SphereAccelerationStructure() : step_size(SPHERE_ACCELERATION_STRUCTURE_STEP_SIZE), n_spheres_x(0), n_spheres_z(0) {}
//    inline int numSpheresX() {return m_spheres.size(); }
//    inline int numSpheresy()
//    {
//        if(m_spheres.size() > 0 )
//            return m_spheres[0].size();

//        return 0;
//    }

    inline void clear() { m_spheres.clear(); }

    const int step_size;
    int n_spheres_x;
    int n_spheres_z;

    // Spheres ordered as x,y
    std::vector<std::vector<Sphere> > m_spheres;
};

class Terrain : public GlDrawable
{
public:
    Terrain();
    ~Terrain();
    virtual bool bindBuffers();
    bool setTerrain(TerragenFile parsed_terrangen_file);
    float getMaxHeight() const { return m_terragen_file.m_header_data.max_height; }
    float getBaseHeight() const { return m_terragen_file.m_header_data.base_height; }
    GLenum getHeightMapTextureUnit() const { return m_htmapTexUnit; }
    const TerrainNormals & getNormals() const {return m_terrain_normals; }
    int getWidth() const {return m_terragen_file.m_header_data.width; }
    int getDepth() const {return m_terragen_file.m_header_data.depth; }
    void normalsRecalculated() { m_terrain_normals.setValid(true); }
    bool traceRay(glm::vec3 start_point, glm::vec3 direction);

    const SphereAccelerationStructure& getSphereAccelerationStructure() { return m_sphere_acceleration_structure; }

private:
    void init();
    void prepare_terrain_geometry();
    void build_sphere_acceleration_structure();
    bool ray_intersect(const glm::vec3 & start, const glm::vec3 & direction, glm::vec3 & intersection_point);
    void refresh_heightmap_texture(TerragenFile & parsed_terrangen_file);

    // Textures
    GLuint m_heightmapTexture; // id of heightmap texture; generated outside class
    GLenum m_htmapTexUnit; // height/normal map - texture units reserved

    TerragenFile m_terragen_file;
    TerrainNormals m_terrain_normals;

    SphereAccelerationStructure m_sphere_acceleration_structure;
};


#endif // TERRAIN_H
