#ifndef TERRAIN_H
#define TERRAIN_H

#include "gl_drawable.h"
#include "terragen/terragen_file_spec.h"

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

class Terrain : public GlDrawable
{
public:
    Terrain();
    ~Terrain();
    virtual bool bindBuffers();
    bool setTerrain(TerragenFile parsed_terrangen_file);
    float getMaxHeight() const { return m_parsed_data.m_header_data.max_height; }
    float getDynamicScale() const { return m_parsed_data.m_header_data.dynamic_scale; }
    float getBaseHeight() const { return m_parsed_data.m_header_data.base_height; }
    GLenum getHeightMapTextureUnit() const { return m_htmapTexUnit; }
    const TerrainNormals & getNormals() const {return m_terrain_normals; }
    int getWidth() const {return m_parsed_data.m_header_data.width; }
    int getDepth() const {return m_parsed_data.m_header_data.depth; }
    void normalsRecalculated() { m_terrain_normals.setValid(true); }

private:
    void init();
    void prepare_terrain_geometry();

    // Textures
    GLuint m_heightmapTexture; // id of heightmap texture; generated outside class
    GLenum m_htmapTexUnit; // height/normal map - texture units reserved

    TerragenFile m_parsed_data;
    TerrainNormals m_terrain_normals;
};


#endif // TERRAIN_H
