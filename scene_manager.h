#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "glm/glm.hpp"
#include <vector>
#include "gl_drawable.h"
#include "shape_holder.h"
#include "terrain.h"
#include "grid_manager.h"

struct SceneAsset{
    DrawData m_draw_data;
    glm::mat4x4 m_mtw_matrix;
};

class SceneManager{

public:
    SceneManager(int terrain_dim) : m_scene_objects(), m_terrain_ready(false), m_terrain_dim(terrain_dim) {}
    std::vector<SceneAsset> getSceneAssets() {return m_scene_objects; }
    DrawData getGridData() { return m_grid_holder.getDrawData(); }
    Terrain& getTerrain() { return m_terrain; }
    void initScene();
    void loadTerrain(QString filename);
    bool terrainReady() { return m_terrain_ready; }
    void setTerrainDim(int dim);
    void genCube(int x, int y, int z);

private:
    std::vector<SceneAsset> m_scene_objects;
    GridHolder m_grid_holder;
    ShapesHolder m_shape_holder;
    Terrain m_terrain;
    bool m_terrain_ready;
    QString current_terrain_file;

    int m_terrain_dim;
};

#endif //SCENE_MANAGER_H
