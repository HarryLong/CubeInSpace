#include "scene_manager.h"
#include <glm/gtc/matrix_transform.hpp>
#include "grid_manager.h"
#include "terragen/terragen_file_manager.h"

#include "utils/utils.h"


void SceneManager::initScene()
{
    // Generate the grid
    m_grid_holder.genGrid();
    m_grid_holder.bindBuffers();
}

SceneAsset SceneManager::genCube(const glm::vec3 & center, float scale)
{
    SceneAsset scene_object(
                m_shape_holder.get(Shape::Cube)->getDrawData(), // Draw data
                glm::translate(glm::mat4x4(), center), // MTW matrix
                scale);

    return scene_object;
}

void SceneManager::loadTerrain(QString filename)
{
    current_terrain_file = filename;
    // Read terragen file
    TerragenFile terragen_file(TerragenFileManager::readTerragen(filename.toStdString()));

    // Scale terragen file
    float scale (((float)m_terrain_width) / terragen_file.m_header_data.width);
    std::cout << "Scale: " << scale << std::endl;
//    TerragenFileManager::scale(terragen_file,scale);
    m_terrain_ready = m_terrain.setTerrain(terragen_file);

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

void SceneManager::setTerrainDim(int dim)
{
    if(m_terrain_width != dim)
    {
        m_terrain_width = dim;
        // Reload the terrain
        if(!current_terrain_file.isNull())
            loadTerrain(current_terrain_file);
    }
}
