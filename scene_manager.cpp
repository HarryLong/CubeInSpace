#include "scene_manager.h"

#include "glm_rotations.h"
#include "grid.h"
#include "glm_utils.h"
#include <QSlider>
#include "include/terragen_file_manager/terragen_file_manager.h"

SceneManager::SceneManager(QSlider * latitude_slider, QSlider * time_of_day_slider, QSlider * month_slider, int terrain_scale) :
    m_terrain_scale(terrain_scale), m_sun(NULL), m_orientation_compass(),
    m_lighting_manager(m_orientation_compass.getNorthOrientation(), m_orientation_compass.getTrueNorthOrientation(),
                       m_orientation_compass.getEastOrientation())
{
    establish_connections(latitude_slider, time_of_day_slider, month_slider);
}

SceneManager::~SceneManager()
{
    clear_acceleration_structure_viewer();
    delete m_sun;
}

void SceneManager::establish_connections(QSlider *latitude_slider, QSlider *time_of_day_slider, QSlider *month_slider)
{
    connect(latitude_slider, SIGNAL(valueChanged(int)), &m_orientation_compass, SLOT(setLatitude(int))); // Latitude slider controls the orientation compass
    connect(time_of_day_slider, SIGNAL(valueChanged(int)), &m_lighting_manager, SLOT(setTime(int))); // Time controller should change light
    connect(month_slider, SIGNAL(valueChanged(int)),  &m_lighting_manager, SLOT(setMonth(int))); // Month controller should change light

    // When an orientation change occurs, the lighting manager needs to be alerted to change the sun position
    connect(&m_orientation_compass, SIGNAL(orientationChanged(float,float,float,float,float,float,float,float,float)), &m_lighting_manager,
            SLOT(setOrientation(float,float,float,float,float,float,float,float,float)));

    // When the sun position changes we must recalculate shade and re-render
    connect(&m_lighting_manager, SIGNAL(sunPositionChanged(float,float,float)), this, SLOT(sunPositionChanged(float,float,float)));
}

const std::vector<const Asset*> SceneManager::getAccelerationStructure() const
{
    return std::vector<const Asset*>(m_acceleration_structure_viewer.begin(), m_acceleration_structure_viewer.end());
}

Grid & SceneManager::getGrid()
{
    return m_grid;
}

Terrain& SceneManager::getTerrain()
{
    return m_terrain;
}

LightingManager & SceneManager::getLightingManager()
{
    return m_lighting_manager;
}

OrientationCompass & SceneManager::getOrientationCompass()
{
    return m_orientation_compass;
}

void SceneManager::initScene()
{
    // Set mock terrain (flat)
    refresh_base_terrain();
}

void SceneManager::loadTerrain(QString filename)
{
    current_terrain_file = filename;
    // Read terragen file

    TerragenFile terragen_file(filename.toStdString());
    set_terrain(terragen_file);
}

void SceneManager::refreshAccelerationStructureViewer()
{
    clear_acceleration_structure_viewer();

    for(std::vector<SphereAccelerationStructure::Sphere> spheres : m_terrain.getSphereAccelerationStructure().m_spheres)
    {
        for(SphereAccelerationStructure::Sphere sphere : spheres)
        {
            GlCube * cube ( new GlCube(ShapeFactory::getCube(sphere.radius*2)));
            cube->setMtwMat(glm::translate(glm::mat4x4(), sphere.center));
            m_acceleration_structure_viewer.push_back(cube);
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

void SceneManager::sunPositionChanged(float pos_x, float pos_y, float pos_z)
{
    m_terrain.setSunPosition(pos_x, pos_y, pos_z);
    emit refreshRender();
}

#define DEFAULT_TERRAIN_SIZE 200
void SceneManager::refresh_base_terrain()
{
    TerragenFile dummy_terragen_file;
    int width(m_terrain_scale * DEFAULT_TERRAIN_SIZE);
    int height(m_terrain_scale * DEFAULT_TERRAIN_SIZE);

    dummy_terragen_file.m_height_data = (float*) malloc(sizeof(float) * width * height);
    memset(dummy_terragen_file.m_height_data, 0, width * height);

    dummy_terragen_file.m_header_data.base_height = 0;
    dummy_terragen_file.m_header_data.depth = DEFAULT_TERRAIN_SIZE*m_terrain_scale;
    dummy_terragen_file.m_header_data.width = DEFAULT_TERRAIN_SIZE*m_terrain_scale;
    dummy_terragen_file.m_header_data.height_scale = 1.0f;
    dummy_terragen_file.m_header_data.max_height = .0f;
    dummy_terragen_file.m_header_data.min_height = .0f;
    dummy_terragen_file.m_header_data.scale = 30.0f;

    set_terrain(dummy_terragen_file);
}

void SceneManager::set_terrain(TerragenFile & terragen_file)
{
    // Scale terragen file
    if(m_terrain_scale != 1)
        TerragenFileManager::scale(terragen_file, m_terrain_scale);

    m_terrain.setTerrain(terragen_file);
    refreshAccelerationStructureViewer();
    m_lighting_manager.setTerrainDimensions(terragen_file.m_header_data.width, terragen_file.m_header_data.depth);

    glm::vec2 terrain_center(m_terrain.getCenter());
    m_orientation_compass.setCenterPosition(glm::vec3(terrain_center[0], m_terrain.getMaxHeight()+5, terrain_center[1]));
}

#define SUN_RADIUS 20
#define SUN_SLICES 20
#define SUN_STACKS 20
const Asset* SceneManager::getSun()
{
    if(!m_sun)
        m_sun = new GlSphere(ShapeFactory::getSphere(SUN_RADIUS, SUN_SLICES, SUN_STACKS, glm::vec4(1,1,0,1)));

    m_sun->setMtwMat(glm::translate(glm::mat4x4(), glmutils::toVec3(m_lighting_manager.getSunlightProperties().getPosition())));

    return m_sun;
}

/***********
 * DELETES *
 ***********/
void SceneManager::clear_acceleration_structure_viewer()
{
    for(GlCube * cube : m_acceleration_structure_viewer)
        delete cube;
    m_acceleration_structure_viewer.clear();
}
