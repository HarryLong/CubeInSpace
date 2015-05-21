#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "glm/glm.hpp"
#include <vector>
#include "gl_drawable.h"
#include "shape_holder.h"
#include "terrain.h"
#include "orientation_compass.h"
#include "lighting_manager.h"

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
    SceneManager(int terrain_scale) : m_scene_objects(), m_terrain_scale(terrain_scale) {}
    const std::vector<SceneAsset>& getSceneAssets() {return m_scene_objects; }
    const std::vector<SceneAsset>& getAccelerationStructure() {return m_acceleration_structure_viewer; }
    Terrain& getTerrain() { return m_terrain; }
    LightingManager & getLightingManager() { return m_lighting_manager; }
    OrientationCompass & getOrientationCompass() { return m_orientation_compass; }
    void initScene();
    void loadTerrain(QString filename);
    void setTerrainScaler(float p_scale);
    SceneAsset genCube(const glm::vec3 & center, float scale = 1.0f);
    void refreshAccelerationStructureViewer();

private:    
    void refresh_base_terrain();

    std::vector<SceneAsset> m_scene_objects;
    std::vector<SceneAsset> m_acceleration_structure_viewer;

    ShapesHolder m_shape_holder;
    Terrain m_terrain;
    OrientationCompass m_orientation_compass;
    LightingManager m_lighting_manager;

    QString current_terrain_file;

    int m_terrain_scale;
};

#endif //SCENE_MANAGER_H
