#include "terrain_water.h"
#include <cstring>

/***************************
 * TERRAIN WATER HEIGHTMAP *
 ***************************/
TerrainWaterHeightmap::TerrainWaterHeightmap() :
    TextureElement<GLfloat>(QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32),
    m_balanced(true), m_balancing(false), m_balancing_iterations(0)
{

}

TerrainWaterHeightmap::~TerrainWaterHeightmap()
{

}

bool TerrainWaterHeightmap::balanced()
{
    return m_balanced.load();
}

bool TerrainWaterHeightmap::isBalancing()
{
    return m_balancing.load();
}

void TerrainWaterHeightmap::setBalancing(bool balancing)
{
    m_balancing.store(balancing);
}

void TerrainWaterHeightmap::setBalanced(bool balanced)
{
    if(!balanced)
    {
        m_balancing_iterations = 0;
        m_balance_iterations_changes_total = 0;
        m_balance_iterations_changes.clear();
    }
    else
    {
        syncFromGPU();
    }
    m_balanced = balanced;
}

void TerrainWaterHeightmap::incrementBalancingIteration(int changes)
{
    m_balancing_iterations++;
    m_balance_iterations_changes.push_back(changes);
    m_balance_iterations_changes_total += changes;
    perform_balancing_check();
}

void TerrainWaterHeightmap::perform_balancing_check()
{
    int iteration_idx(m_balance_iterations_changes.size()-1);

    if(m_balance_iterations_changes.at(iteration_idx) <= (m_balance_iterations_changes_total/1000.0f))
    {
        setBalanced(true);
    }
}

void TerrainWaterHeightmap::setHeight(const glm::ivec2 point, GLfloat height)
{
    glm::ivec2 p = glm::ivec2(point.x, point.y);
    int index(p.y*m_width+p.x);
    m_raw_data[index] = height;
}

/*****************
 * TERRAIN WATER *
 *****************/
const glm::vec4 TerrainWater::MaterialProperties::_DIFFUSE = glm::vec4(0.f, 0.f, 1.f, 1.0f);
const glm::vec4 TerrainWater::MaterialProperties::_SPECULAR = glm::vec4(0.f, 0.f, 1.f, 1.0f);
const glm::vec4 TerrainWater::MaterialProperties::_AMBIENT = glm::vec4(0.f, 0.f, 1.f, 1.0f);
TerrainWater::TerrainWater()
{

}

TerrainWater::~TerrainWater()
{

}

TerrainWaterHeightmap& TerrainWater::operator[](int month)
{
    return m_terrain_water[month-1];
}

void TerrainWater::setData(GLfloat * data[12], int width, int height)
{
    for(int i = 0; i < 12; i++)
    {
        m_terrain_water[i].setData(data[i], width, height);
    }
}

bool TerrainWater::balanced()
{
    for(TerrainWaterHeightmap& water_heightmap : m_terrain_water)
    {
        if(!water_heightmap.balanced())
            return false;
    }

    return true;
}

std::vector<TerrainWaterHeightmap*> TerrainWater::getUnbalanced()
{
    std::vector<TerrainWaterHeightmap*> unbalanced;
    for(TerrainWaterHeightmap& water_heightmap : m_terrain_water)
    {
        if(!water_heightmap.balanced())
            unbalanced.push_back(&water_heightmap);
    }

    return unbalanced;
}

void TerrainWater::pushToGPU()
{
    for(TerrainWaterHeightmap& water_heightmap : m_terrain_water)
    {
        water_heightmap.pushToGPU();
    }
}

void TerrainWater::reset(int width, int depth)
{
    int sz(sizeof(GLfloat) * width * depth);

    for(TerrainWaterHeightmap & heightmap : m_terrain_water)
    {
        GLfloat * data = (GLfloat *) std::malloc(sz);
        std::memset(data, 0, sz);
        heightmap.setData(data, width, depth);
        heightmap.setBalanced(false);
    }
    pushToGPU();
}
