#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "glm/glm.hpp"
#include <vector>
#include "gl_drawable.h"
#include "shape_holder.h"
#include "terrain.h"
#include "grid_manager.h"

struct SceneAsset{
    SceneAsset(DrawData draw_data, glm::mat4x4 mtw_matrix, float scale = 1.0f) :
        m_draw_data(draw_data),
        m_mtw_matrix(mtw_matrix),
        m_scale(scale) {}

    DrawData m_draw_data;
    glm::mat4x4 m_mtw_matrix;
    float m_scale;
};

class SceneManager{

public:
    SceneManager(int terrain_width) : m_scene_objects(), m_terrain_ready(false), m_terrain_width(terrain_width) {}
    const std::vector<SceneAsset>& getSceneAssets() {return m_scene_objects; }
    const std::vector<SceneAsset>& getAccelerationStructure() {return m_acceleration_structure_viewer; }
    DrawData getGridData() { return m_grid_holder.getDrawData(); }
    Terrain& getTerrain() { return m_terrain; }
    void initScene();
    void loadTerrain(QString filename);
    bool terrainReady() { return m_terrain_ready; }
    void setTerrainDim(int dim);
    SceneAsset genCube(const glm::vec3 & center, float scale = 1.0f);
    void refreshAccelerationStructureViewer();

private:
    std::vector<SceneAsset> m_scene_objects;
    std::vector<SceneAsset> m_acceleration_structure_viewer;

    GridHolder m_grid_holder;
    ShapesHolder m_shape_holder;
    Terrain m_terrain;
    bool m_terrain_ready;
    QString current_terrain_file;

    int m_terrain_width;
};

#endif //SCENE_MANAGER_H
