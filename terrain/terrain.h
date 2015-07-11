#ifndef TERRAIN_H
#define TERRAIN_H

#include "../gl_core/gl_drawable.h"
#include "include/terragen_file_manager/terragen_file.h"
#include "constants.h"
#include "../gl_assets/asset.h"
#include "acceleration_structure.h"
#include "terrain_normals.h"
#include "drawable_terrain.h"

#include <QOpenGLTexture>
#include <glm/common.hpp>


/****************
 * BASE TERRAIN *
 ****************/
class BaseTerrain : public TerragenFile
{
public:
    BaseTerrain();
    virtual ~BaseTerrain();
};

/***********
 * TERRAIN *
 ***********/
class GLSelectionRect;
class Terrain : public QObject
{
    Q_OBJECT    
public:
    Terrain();
    ~Terrain();

    Asset* getSelectionRect();
    void setTerrain(TerragenFile parsed_terrangen_file);
    float getMaxHeight() const;
    float getBaseHeight() const;
    int getWidth() const;
    int getDepth() const;
    bool traceRay(const glm::vec3 & start_point, const glm::vec3 & direction, glm::vec3 & intersection_point, bool search_closest = true);
    void setSelectionRectangle(glm::vec3 min, glm::vec3 max);
    const SphericalAccelerationStructure<Hierarchical>& getSphereAccelerationStructure();
    void loadBaseTerrain();
    void getTerrainDimensions(int & width, int & depth, int & base_height, int & max_height );
    void render();
    // Terrain normals functions
    void calculateNormals();
    bool normalsValid();
    float getHeight(const glm::vec2 & point) const;
    float getAltitude(const glm::vec2 & point) const;
    float getSlope(const glm::vec2 & point) const;

    const TerragenFile & getHeightMap();

    DrawableTerrain & getDrawableTerrain();
    TerrainNormals & getNormals();

    struct MaterialProperties{
        static const glm::vec4 _DIFFUSE;
        static const glm::vec4 _SPECULAR;
        static const glm::vec4 _AMBIENT;
    };

signals:
    void terrainDimensionsChanged(int width, int depth, int base_height, int max_height);

private:
    void establish_connections();

    void clear_selection_rectangle();

    TerragenFile m_terragen_file;
    SphericalAccelerationStructure<Hierarchical> m_sphere_acceleration_structure;
    GLSelectionRect * m_selection_rectangle;
    DrawableTerrain m_drawable_terrain;
    TerrainNormals m_terrain_normals;
};

#endif // TERRAIN_H
