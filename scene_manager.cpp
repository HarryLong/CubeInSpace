#include "scene_manager.h"
#include <glm/gtc/matrix_transform.hpp>
#include "grid_manager.h"
#include "terragen/terragen_file_manager.h"

#include "utils/utils.h"


void SceneManager::initScene()
{
    // Set mock terrain (flat)
    refresh_base_terrain();
}

SceneAsset SceneManager::genCube(const glm::vec3 & center, float scale)
{
    SceneAsset cube_asset(
                m_shape_holder.get(Shape::Cube)->getDrawData(), // Draw data
                glm::translate(glm::mat4x4(), center), // MTW matrix
                scale);

    return cube_asset;
}

void SceneManager::loadTerrain(QString filename)
{
    current_terrain_file = filename;
    // Read terragen file

    TerragenFile terragen_file(TerragenFileManager::readTerragen(filename.toStdString()));

    // Scale terragen file
    if(m_terrain_scale != 1)
        TerragenFileManager::scale_factor(terragen_file, m_terrain_scale);

    m_terrain.setTerrain(terragen_file);
    refreshAccelerationStructureViewer();
}

void SceneManager::refreshAccelerationStructureViewer()
{
    m_acceleration_structure_viewer.clear();
    for(std::vector<Sphere> spheres : m_terrain.getSphereAccelerationStructure().m_spheres)
    {
        for(Sphere sphere : spheres)
        {
            m_acceleration_structure_viewer.push_back(genCube(sphere.center, sphere.radius * 2));
        }
    }
}

void SceneManager::setTerrainScaler(float p_scale)
{
    if(m_terrain_scale != p_scale)
    {
        m_terrain_scale = p_scale;
        // Reload the terrain
        if(!current_terrain_file.isNull())
            loadTerrain(current_terrain_file);
        else
            refresh_base_terrain();
    }
}

#define DEFAULT_TERRAIN_SIZE 1024
void SceneManager::refresh_base_terrain()
{
    TerragenFile dummy_terragen_file;
    int width(m_terrain_scale * DEFAULT_TERRAIN_SIZE);
    int height(m_terrain_scale * DEFAULT_TERRAIN_SIZE);

    dummy_terragen_file.m_height_data = (float*) malloc(sizeof(float) * width * height);
    memset(dummy_terragen_file.m_height_data, 0, width * height);

    dummy_terragen_file.m_header_data.base_height = 0;
    dummy_terragen_file.m_header_data.depth = 1024*m_terrain_scale;
    dummy_terragen_file.m_header_data.width = 1024*m_terrain_scale;
    dummy_terragen_file.m_header_data.height_scale = 1.0f;
    dummy_terragen_file.m_header_data.max_height = .0f;
    dummy_terragen_file.m_header_data.min_height = .0f;
    dummy_terragen_file.m_header_data.scale = 30.0f;

    m_terrain.setTerrain(dummy_terragen_file);

    refreshAccelerationStructureViewer();
}
