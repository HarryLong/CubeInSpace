#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "glm/glm.hpp"
#include <vector>
#include "gl_drawable.h"
#include "terrain.h"
#include "orientation_compass.h"
#include "lighting_manager.h"
#include "grid.h"

class SceneManager{
public:
    SceneManager(int terrain_scale);
    ~SceneManager();

    const std::vector<const Asset*> getAccelerationStructure() const;

//    SceneAsset* getSun() const;
    Terrain& getTerrain();
    LightingManager & getLightingManager();
    OrientationCompass & getOrientationCompass();
    Grid & getGrid();

    void initScene();
    void loadTerrain(QString filename);
    void setTerrainScaler(float p_scale);
    void refreshAccelerationStructureViewer();

private:    
    void refresh_base_terrain();

    void clear_acceleration_structure_viewer();

    std::vector<GlCube*> m_acceleration_structure_viewer;
    Grid m_grid;
    Terrain m_terrain;
    OrientationCompass m_orientation_compass;
    LightingManager m_lighting_manager;

    QString current_terrain_file;

    int m_terrain_scale;
};

#endif //SCENE_MANAGER_H
