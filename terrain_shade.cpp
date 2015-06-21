#include "terrain_shade.h"

/*****************
 * TERRAIN SHADE *
 *****************/
TerrainShade::TerrainShade() :
    TextureElement<GLubyte>(QOpenGLTexture::TextureFormat::RG8_UNorm, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::UInt8)
{

}

TerrainShade::~TerrainShade()
{

}
