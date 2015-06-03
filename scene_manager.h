#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "glm/glm.hpp"
#include <vector>
#include "gl_drawable.h"
#include "terrain.h"
#include "orientation_compass.h"
#include "lighting_manager.h"
#include "grid.h"

#include <QObject>

class QSlider;

class SceneManager : public QObject{
Q_OBJECT
public:
    SceneManager(QSlider * latitude_slider, QSlider * time_of_day_slider, QSlider * month_slider, int terrain_scale);
    ~SceneManager();

    const std::vector<const Asset*> getAccelerationStructure() const;

    const Asset* getSun();
    Terrain& getTerrain();
    LightingManager & getLightingManager();
    OrientationCompass & getOrientationCompass();
    Grid & getGrid();

    void initScene();
    void loadTerrain(QString filename);
    void setTerrainScaler(float p_scale);
    void refreshAccelerationStructureViewer();

signals:
    void refreshRender();

public slots:
    void sunPositionChanged(float pos_x, float pos_y, float pos_z);

private:    
    void refresh_base_terrain();
    void set_terrain(TerragenFile & terragen_file);

    void clear_acceleration_structure_viewer();

    void establish_connections(QSlider * latitude_slider, QSlider * time_of_day_slider, QSlider * month_slider );

    std::vector<GlCube*> m_acceleration_structure_viewer;
    GlSphere * m_sun;
    Grid m_grid;
    Terrain m_terrain;
    OrientationCompass m_orientation_compass;
    LightingManager m_lighting_manager;

    QString current_terrain_file;

    int m_terrain_scale;
};

#endif //SCENE_MANAGER_H
