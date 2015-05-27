#include "scene_manager.h"

#define GLM_FORCE_RADIANS

#include <glm/gtc/matrix_transform.hpp>
#include "grid.h"
#include "terragen/terragen_file_manager.h"

#include "utils/utils.h"

SceneManager::SceneManager(int terrain_scale) : m_terrain_scale(terrain_scale)
{

}

SceneManager::~SceneManager()
{
    clear_acceleration_structure_viewer();
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

#define SUN_RADIUS 10
#define SUN_SLICES 10
#define SUN_STACKS 10
//SceneAsset * SceneManager::getSun()
//{
//    if(m_sun_asset == NULL)
//        m_sun_asset = new SceneAsset(ShapeFactory::getSphere(SUN_RADIUS, SUN_SLICES, SUN_STACKS));

//    // Sun position
//    glm::vec4 sun_position(m_lighting_manager.getSunlightProperties().getPosition());
//    m_sun_asset->m_mtw_matrix = glm::translate(glm::mat4x4(), glm::vec3(sun_position[0], sun_position[1], sun_position[2]);

//    return m_sun_asset;
//}

void SceneManager::loadTerrain(QString filename)
{
    current_terrain_file = filename;
    // Read terragen file

    TerragenFile terragen_file(TerragenFileManager::readTerragen(filename.toStdString()));

    // Scale terragen file
    if(m_terrain_scale != 1)
        TerragenFileManager::scale_factor(terragen_file, m_terrain_scale);

    m_terrain.setTerrain(terragen_file);
    refreshAccelerationStructureViewer();
    m_lighting_manager.setTerrainDimensions(terragen_file.m_header_data.width, terragen_file.m_header_data.depth);
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

#define DEFAULT_TERRAIN_SIZE 1024
void SceneManager::refresh_base_terrain()
{
    TerragenFile dummy_terragen_file;
    int width(m_terrain_scale * DEFAULT_TERRAIN_SIZE);
    int height(m_terrain_scale * DEFAULT_TERRAIN_SIZE);

    dummy_terragen_file.m_height_data = (float*) malloc(sizeof(float) * width * height);
    memset(dummy_terragen_file.m_height_data, 0, width * height);

    dummy_terragen_file.m_header_data.base_height = 0;
    dummy_terragen_file.m_header_data.depth = 1024*m_terrain_scale;
    dummy_terragen_file.m_header_data.width = 1024*m_terrain_scale;
    dummy_terragen_file.m_header_data.height_scale = 1.0f;
    dummy_terragen_file.m_header_data.max_height = .0f;
    dummy_terragen_file.m_header_data.min_height = .0f;
    dummy_terragen_file.m_header_data.scale = 30.0f;

    m_terrain.setTerrain(dummy_terragen_file);

    refreshAccelerationStructureViewer();

    m_lighting_manager.setTerrainDimensions(dummy_terragen_file.m_header_data.width, dummy_terragen_file.m_header_data.depth);
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
