#include "terrain.h"
#include <iostream>
#include <cfloat>
#include "geom.h"
#include <math.h>

#include <cfloat>
#include <QProgressDialog>
#include "actions.h"
#include "../gl_assets/shape_factory.h"

//-------------------------------------------------------------------
#define BASE_TERRAIN_SIZE 64
BaseTerrain::BaseTerrain()
{
    int width(BASE_TERRAIN_SIZE);
    int depth(BASE_TERRAIN_SIZE);
    m_height_data = (float*) malloc(sizeof(int) * width * depth);

    memset(m_height_data, 0, sizeof(int) * width * depth);
    m_header_data.base_height = 0;
    m_header_data.depth = BASE_TERRAIN_SIZE;
    m_header_data.width = BASE_TERRAIN_SIZE;
    m_header_data.height_scale = 1.0f;
    m_header_data.max_height = .0f;
    m_header_data.min_height = .0f;
    m_header_data.scale = 30.0f;
}

BaseTerrain::~BaseTerrain()
{

}

/***********
 * TERRAIN *
 ***********/
const glm::vec4 Terrain::MaterialProperties::_DIFFUSE = glm::vec4(0.7f, 0.6f, 0.5f, 1.0f);
const glm::vec4 Terrain::MaterialProperties::_SPECULAR = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
const glm::vec4 Terrain::MaterialProperties::_AMBIENT = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
Terrain::Terrain()
{
    establish_connections();
}

Terrain::~Terrain()
{
}

void Terrain::establish_connections()
{

}

void Terrain::loadBaseTerrain()
{
    setTerrain(BaseTerrain());
}

void Terrain::setTerrain(TerragenFile parsed_terrangen_file)
{
    emit newTerrainGoingToLoad(parsed_terrangen_file.m_header_data.width, parsed_terrangen_file.m_header_data.depth);

    m_terrain_normals.setTerrainDim(parsed_terrangen_file.m_header_data.width, parsed_terrangen_file.m_header_data.depth);
    m_terrain_normals.setValid(false);
    m_sphere_acceleration_structure.build(parsed_terrangen_file);
    m_drawable_terrain.prepareTerrainGeometry(parsed_terrangen_file);

    m_terragen_file = parsed_terrangen_file;
    m_terragen_file.summarize();

    emit normalsInvalid();
    emit newTerrainLoaded(getWidth(), getDepth());
}

float Terrain::getMinHeight() const
{
    return m_terragen_file.m_header_data.min_height;
}

float Terrain::getMaxHeight() const
{
    return m_terragen_file.m_header_data.max_height;
}

float Terrain::getBaseHeight() const
{
    return m_terragen_file.m_header_data.base_height;
}

int Terrain::getWidth() const
{
    return m_terragen_file.m_header_data.width;
}

int Terrain::getDepth() const
{
    return m_terragen_file.m_header_data.depth;
}

const SphericalAccelerationStructure<Hierarchical> & Terrain::getSphereAccelerationStructure()
{
    return m_sphere_acceleration_structure;
}

void Terrain::calculateNormals()
{
    m_terrain_normals.render();
    m_terrain_normals.setValid(true);
}

void Terrain::render()
{
    m_drawable_terrain.render();
}

//bool Terrain::normalsValid()
//{
//    return m_terrain_normals.valid();
//}

void Terrain::getTerrainDimensions(int & width, int & depth, int & base_height, int & max_height )
{
    width = getWidth();
    depth = getDepth();
    base_height = getBaseHeight();
    max_height = getMaxHeight();
}

float Terrain::getHeight(const glm::vec2 & point) const
{
    return m_terragen_file(point[0], point[1]);
}

float Terrain::getHeight(const glm::ivec2 & point) const
{
    return m_terragen_file(point[0], point[1]);
}

float Terrain::getAltitude(const glm::vec2 & point) const
{
    return getHeight(point) * getScale();
}

float Terrain::getAltitude(const glm::ivec2 & point) const
{
    return getHeight(point) * getScale();
}

float Terrain::getScale() const
{
    return m_terragen_file.m_header_data.scale;
}

float Terrain::getSlope(const glm::vec2 & point) const
{
    glm::vec3 normal(m_terrain_normals(point[0],point[1]));
    glm::vec3 vertical_vector(glm::vec3(0,1,0));

    return Geom::toDegrees(std::abs(std::acos(glm::dot(normal, vertical_vector))));
}

bool Terrain::traceRay(const glm::vec3 & start, const glm::vec3 & direction, glm::vec3 & intersection_point, bool search_closest)
{
    // bounding sphere accel structure
    float scaler(.0f), distance(.0f);
    int tolerance (1);
    bool found(false);

    float min_distance(FLT_MAX);

    // Get all possible spheres
    std::vector<AccelerationSphere> intersecting_spheres(m_sphere_acceleration_structure.get_intersecting_spheres(start, direction));

    for(AccelerationSphere intersecting_sphere : intersecting_spheres)
    {
        int x_min(intersecting_sphere.m_center[0] - intersecting_sphere.m_radius);
        int x_max(std::min(getWidth()-1, ((int) intersecting_sphere.m_center[0] + intersecting_sphere.m_radius)));

        int z_min(intersecting_sphere.m_center[2] - intersecting_sphere.m_radius);
        int z_max(std::min(getDepth()-1, ((int) intersecting_sphere.m_center[2] + intersecting_sphere.m_radius)));

        for(int point_in_sphere_x = x_min; point_in_sphere_x < x_max; point_in_sphere_x++)
        {
            for(int point_in_sphere_z = z_min; point_in_sphere_z < z_max; point_in_sphere_z++)
            {
                glm::vec3 point_in_sphere(point_in_sphere_x, m_terragen_file(point_in_sphere_x,point_in_sphere_z),
                                          point_in_sphere_z);
                if(Geom::length(Geom::diff(start, point_in_sphere)) > 5) // Make sure the point is further than 5 unit distance from the start point
                {
                    Geom::rayPointDist(start, direction, point_in_sphere, scaler, distance);
                    if(distance < tolerance && distance < min_distance)
                    {
                        found = true;
                        min_distance = distance;
                        intersection_point = point_in_sphere;
                        if(!search_closest)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return found;
}

DrawableTerrain & Terrain::getDrawableTerrain()
{
    return m_drawable_terrain;
}

TerrainNormals & Terrain::getNormals()
{
    return m_terrain_normals;
}

const TerragenFile & Terrain::getHeightMap()
{
    return m_terragen_file;
}
