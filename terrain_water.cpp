#include "terrain_water.h"

const glm::vec4 TerrainWater::MaterialProperties::_DIFFUSE = glm::vec4(0.f, 0.f, 1.f, 1.0f);
const glm::vec4 TerrainWater::MaterialProperties::_SPECULAR = glm::vec4(0.f, 0.f, 1.f, 1.0f);
const glm::vec4 TerrainWater::MaterialProperties::_AMBIENT = glm::vec4(0.f, 0.f, 1.f, 1.0f);
TerrainWater::TerrainWater() :
    TextureElement<GLuint>(QOpenGLTexture::TextureFormat::R32U, QOpenGLTexture::PixelFormat::Red_Integer, QOpenGLTexture::PixelType::UInt32)
{

}

TerrainWater::~TerrainWater()
{

}

void TerrainWater::tmp_calculateTotalWater()
{
    GLuint total(0);
    for(int i = 0; i < width()*height(); i++)
    {
        total+=m_raw_data[i];
    }
    qCritical() << "Total water: " << total;

    qCritical() << "0,0 --> " << m_raw_data[0];
    qCritical() << "width,0 --> " << m_raw_data[width()-1];
    qCritical() << "0,height --> " << m_raw_data[(width()-1)*height()];
    qCritical() << "width,height --> " << m_raw_data[width()*height()-1];
}
