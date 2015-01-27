#include "scene_manager.h"
#include <glm/gtc/matrix_transform.hpp>
#include "grid_manager.h"
#include "terragen/terragen_file_manager.h"

void SceneManager::initScene()
{
    // Generate the grid
    m_grid_holder.genGrid();
    m_grid_holder.bindBuffers();

    //Generate two cubes
    m_shape_holder.genCube();
    m_shape_holder.bindBuffers();
//    {
//        SceneAsset scene_object;
//        scene_object.m_mtw_matrix = glm::translate(glm::mat4x4(), glm::vec3(-2,0,-2));
//        scene_object.m_draw_data = m_shape_holder.getDrawData();
//        m_scene_objects.push_back(scene_object);
//    }
    {
        SceneAsset scene_object;
        scene_object.m_mtw_matrix = glm::translate(glm::mat4x4(), glm::vec3(2,0,-2));
        scene_object.m_draw_data = m_shape_holder.getDrawData();
        m_scene_objects.push_back(scene_object);
    }
}

void SceneManager::loadTerrain(QString filename)
{
    current_terrain_file = filename;
    m_terrain_ready = m_terrain.setTerrain(readTerragen(filename.toStdString(), m_terrain_dim));
}

void SceneManager::setTerrainDim(int dim)
{
    if(m_terrain_dim != dim)
    {
        m_terrain_dim = dim;
        // Reload the terrain
        if(!current_terrain_file.isNull())
            loadTerrain(current_terrain_file);
    }
}
