#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "glm/glm.hpp"
#include <vector>
#include "gl_drawable.h"
#include "terrain.h"
#include "lighting_manager.h"
#include "grid.h"

#include <QObject>
#include "controllers.h"
#include "progress_bar_widget.h"

class QSlider;
class ActionFamily;
class RainfallEditDialog;
class SceneManager : public QObject{
Q_OBJECT
public:
    SceneManager(PositionControllers position_controllers, TimeControllers time_controllers, TerrainControllers terrain_controllers,
                 TemperatureEditDialog * temp_edit_dlg, RainfallEditDialog * rainfall_edit_dlg, ActionFamily * overlay_actions);
    ~SceneManager();

    Asset* getAccelerationStructure();
    Asset* getSun();
    Terrain * getTerrain();
    LightingManager & getLightingManager();
    Grid & getGrid();

    void loadTerrain(QString filename);
    void initScene();

signals:
    void processing(QString description);
    void processingUpdate(int percent_complete);
    void processingComplete();

public slots:
    void sunPositionChanged(float pos_x, float pos_y, float pos_z);
    void setNorthOrientation(float north_x, float north_y, float north_z);
    void refreshAccelerationStructureViewer();
    void refreshDailyIllumination();

private slots:
    void emitProcessing(QString description);
    void emitProcessingComplete();
    void emitProcessingUpdate(int update);

private:    
//    void clear_acceleration_structure_viewer();
    void establish_connections();

    Asset * m_acceleration_structure_viewer;
    Asset * m_sun;
    Grid m_grid;
    Terrain * m_terrain; // Pointer to control
    LightingManager m_lighting_manager;

    QString current_terrain_file;

//    TemperatureEditDialog * m_temp_edit_dlg;
};

#endif //SCENE_MANAGER_H
