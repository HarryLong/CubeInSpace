#include "soil_humidity.h"
#include <cstring>

//-----------------------------------------------------------------------------------------------

SoilHumidity::SoilHumidity() :
    TextureElement2DArray<GLuint>(12, QOpenGLTexture::TextureFormat::R32U, QOpenGLTexture::PixelFormat::Red_Integer, QOpenGLTexture::PixelType::UInt32)
{

}

SoilHumidity::~SoilHumidity()
{

}
