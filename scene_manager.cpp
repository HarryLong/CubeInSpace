#include "scene_manager.h"

#include "glm_wrapper.h"
#include "grid.h"
#include <QSlider>
#include "include/terragen_file_manager/terragen_file_manager.h"
#include <cstring>
#include "temp_edit_dlg.h"
#include "orientation_widget.h"
#include "shape_factory.h"

SceneManager::SceneManager(PositionControllers position_controllers, TimeControllers time_controllers, TerrainControllers terrain_controllers,
                           TemperatureEditDialog * temp_edit_dlg, Actions * overlay_actions) :
    m_sun(NULL), m_acceleration_structure_viewer(NULL),  m_lighting_manager(time_controllers, position_controllers),
    m_terrain(new Terrain(terrain_controllers, temp_edit_dlg, overlay_actions))
{
    establish_connections();
}

SceneManager::~SceneManager()
{
    delete m_terrain;
    if(m_sun)
        delete m_sun;
    if(m_acceleration_structure_viewer)
        delete m_acceleration_structure_viewer;
}

void SceneManager::initScene()
{
    m_terrain->loadBaseTerrain();
}

void SceneManager::establish_connections()
{
    connect(m_terrain, SIGNAL(terrainDimensionsChanged(int,int,int,int)), &m_lighting_manager, SLOT(setTerrainDimensions(int,int,int,int)));
    connect(m_terrain, SIGNAL(terrainDimensionsChanged(int,int,int,int)), this, SLOT(refreshAccelerationStructureViewer()));

    // When the sun position changes we must recalculate shade and re-render
    connect(&m_lighting_manager, SIGNAL(sunPositionChanged(float,float,float)), this, SLOT(sunPositionChanged(float,float,float)));

    // Recalculate illumination recalculation
    connect(m_terrain, SIGNAL(trigger_daily_illumination_refresh()), this, SLOT(refreshDailyIllumination()));

    // PROGRESS BAR CONNECTIONS //
    // When the terrain is busy, a processing signal needs to be emitted
    connect(m_terrain, SIGNAL(processing(QString)), this, SLOT(emitProcessing(QString)));

    // The progress must be updated
    connect(m_terrain, SIGNAL(intermediate_processing_update(int)), this, SLOT(emitProcessingUpdate(int)));

    // When the terrain has finished. a finished processing signal should be emitted
    connect(m_terrain, SIGNAL(processing_complete()), this, SLOT(emitProcessingComplete()));
}

Asset * SceneManager::getAccelerationStructure()
{
    return m_acceleration_structure_viewer;
}

Grid & SceneManager::getGrid()
{
    return m_grid;
}

Terrain * SceneManager::getTerrain()
{
    return m_terrain;
}

LightingManager & SceneManager::getLightingManager()
{
    return m_lighting_manager;
}

void SceneManager::loadTerrain(QString filename)
{
    current_terrain_file = filename;
    // Read terragen file
    TerragenFile parsed_terragen_file(filename.toStdString());
    m_terrain->setTerrain(parsed_terragen_file);
}

void SceneManager::refreshAccelerationStructureViewer()
{
    float terrain_scale(m_terrain->getScale());
    if(!m_acceleration_structure_viewer)
        m_acceleration_structure_viewer = ShapeFactory::getCube(1.0f);
    m_acceleration_structure_viewer->clearTransformations();
    for(const AccelerationSphere & sphere : m_terrain->getSphereAccelerationStructure().getSpheres())
    {
        glm::mat4x4 mtw(glm::translate(glm::mat4x4(), sphere.m_center*terrain_scale));
        float scale(sphere.m_radius*2.f*terrain_scale);
        m_acceleration_structure_viewer->addTransformation(mtw, scale);
    }
}

void SceneManager::sunPositionChanged(float pos_x, float pos_y, float pos_z)
{
    m_terrain->setSunPosition(pos_x, pos_y, pos_z);
}

void SceneManager::emitProcessing(QString description)
{
    emit processing(description);
}

void SceneManager::emitProcessingUpdate(int update)
{
    emit processingUpdate(update);
}

void SceneManager::emitProcessingComplete()
{
    emit processingComplete();
}

void SceneManager::refreshDailyIllumination()
{
    int current_month(m_lighting_manager.currentMonth());
    int current_time(m_lighting_manager.currentTime());

    int terrain_width(m_terrain->getWidth());
    int terrain_depth(m_terrain->getDepth());
    int n_elements(terrain_width*terrain_depth);

    GLubyte * intermediary_illumination_data = (GLubyte*) malloc(sizeof(GLubyte)*n_elements);
    GLubyte * aggregated_min = (GLubyte*) malloc(sizeof(GLubyte)*n_elements);
    GLubyte * aggregated_max = (GLubyte*) malloc(sizeof(GLubyte)*n_elements);

    for(int month = 1; month < 13; month++)
    {
        m_lighting_manager.setMonth(month);
        memset(intermediary_illumination_data, 0x00, n_elements);

        for(int hour = 0; hour < 24; hour++)
        {
            m_lighting_manager.setTime(hour * 60);
            m_terrain->refreshShade("Calculating shade for: Month " + QString::number(month) + " | Hour: " + QString::number(hour));
            const GLubyte * shade_data(m_terrain->getShade()->getRawData());
#pragma omp parallel for
            for(int z = 0 ; z < terrain_depth; z++)
            {
                for(int x = 0; x < terrain_width; x++)
                {
                    int index(z*terrain_width+x);
                    intermediary_illumination_data[index] += (shade_data[index] > 0 ? 0 : 1);
                }
            }
        }
        // Now check if it is the minimum/maximum
        if(month > 1)
        {
#pragma omp parallel for
            for(int z = 0 ; z < terrain_depth; z++)
            {
                for(int x = 0; x < terrain_width; x++)
                {
                    int index(z*terrain_width+x);
                    GLubyte value(intermediary_illumination_data[index]);
                    if(value > aggregated_max[index])
                        aggregated_max[index] = value;
                    if(value < aggregated_min[index])
                        aggregated_min[index] = value;
                }
            }
        }
        else
        {
            std::memcpy(aggregated_min, intermediary_illumination_data, n_elements);
            std::memcpy(aggregated_max, intermediary_illumination_data, n_elements);
        }
    }
    free(intermediary_illumination_data);
    m_terrain->setMinIlluminationData(aggregated_min);
    m_terrain->setMaxIlluminationData(aggregated_max);

    // Restore time and date
    m_lighting_manager.setMonth(current_month);
    m_lighting_manager.setTime(current_time);
}

#define SUN_RADIUS 20
#define SUN_SLICES 20
#define SUN_STACKS 20
Asset* SceneManager::getSun()
{
    if(!m_sun)
        m_sun = ShapeFactory::getSphere(SUN_RADIUS, SUN_SLICES, SUN_STACKS, glm::vec4(1,1,0,1));

    glm::vec4 sun_position (m_lighting_manager.getSunlightProperties().getPosition());
    glm::mat4x4 mtw( glm::translate(glm::mat4x4(), glm::vec3(sun_position.x, sun_position.y, sun_position.z)));
    m_sun->setTranformation(mtw);

    return m_sun;
}

void SceneManager::setNorthOrientation(float north_x, float north_y, float north_z)
{
    m_lighting_manager.setNorthOrientation(north_x, north_y, north_z);
    m_terrain->invalidateIllumination(); // Illumination data needs to be recalculated
}

/***********
 * DELETES *
 ***********/
//void SceneManager::clear_acceleration_structure_viewer()
//{
//    for(GlCube * cube : m_acceleration_structure_viewer)
//        delete cube;
//    m_acceleration_structure_viewer.clear();
//}
