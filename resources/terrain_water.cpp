#include "terrain_water.h"

const glm::vec4 TerrainWater::MaterialProperties::_DIFFUSE = glm::vec4(0.f, 0.f, 1.f, 1.0f);
const glm::vec4 TerrainWater::MaterialProperties::_SPECULAR = glm::vec4(0.f, 0.f, 1.f, 1.0f);
const glm::vec4 TerrainWater::MaterialProperties::_AMBIENT = glm::vec4(0.f, 0.f, 1.f, 1.0f);
TerrainWater::TerrainWater() :
    m_balanced(true), m_balancing(false), m_balancing_iterations(0)
{

}

TerrainWater::~TerrainWater()
{

}

bool TerrainWater::isBalancing()
{
    return m_balancing.load();
}

void TerrainWater::setBalancing(bool balancing)
{
    if(balancing)
    {
        m_balancing_mutex.lock();
    }
    else
    {
        m_balancing_mutex.unlock();
    }

    m_balancing.store(balancing);
}

void TerrainWater::pushToGPU()
{
    m_terrain_water_jun.pushToGPU();
    m_terrain_water_dec.pushToGPU();
}

void TerrainWater::setData(GLuint * jun_data, GLuint * dec_data, int width, int height)
{
    m_terrain_water_jun.setData(jun_data, width, height);
    m_terrain_water_dec.setData(dec_data, width, height);
    set_balanced(false);
}

void TerrainWater::bindDec()
{
    m_terrain_water_dec.bind();
}

void TerrainWater::bindJun()
{
    m_terrain_water_jun.bind();
}

GLuint TerrainWater::getDecTextureId()
{
    return m_terrain_water_dec.textureId();
}

GLuint TerrainWater::getJunTextureId()
{
    return m_terrain_water_jun.textureId();
}

void TerrainWater::syncFromGPU()
{
    m_terrain_water_jun.syncFromGPU();
    m_terrain_water_dec.syncFromGPU();
}

bool TerrainWater::balanced()
{
    return m_balanced.load();
}

void TerrainWater::set_balanced(bool balanced)
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

void TerrainWater::getWaterData(int x, int z, GLuint & jun, GLuint & dec)
{
    jun = m_terrain_water_jun(x,z);
    dec = m_terrain_water_dec(x,z);
}

void TerrainWater::incrementBalancingIteration(int changes)
{
    m_balancing_iterations++;
    m_balance_iterations_changes.push_back(changes);
    m_balance_iterations_changes_total += changes;
    perform_balancing_check();
}

const GLuint * TerrainWater::getJunRawData()
{
    return m_terrain_water_jun.getRawData();
}

const GLuint * TerrainWater::getDecRawData()
{
    return m_terrain_water_dec.getRawData();
}

void TerrainWater::perform_balancing_check()
{
    int iteration_idx(m_balance_iterations_changes.size()-1);

    if(m_balance_iterations_changes.at(iteration_idx) <= (m_balance_iterations_changes_total/1000.0f))
    {
        set_balanced(true);
    }
}

TerrainWaterHeightmap::TerrainWaterHeightmap() :
    TextureElement<GLuint>(QOpenGLTexture::TextureFormat::R32U, QOpenGLTexture::PixelFormat::Red_Integer, QOpenGLTexture::PixelType::UInt32)
{

}

TerrainWaterHeightmap::~TerrainWaterHeightmap()
{

}
