#include "terrain_shade.h"
#include "../terrain/terrain.h"
#include "../terrain/terrain_normals.h"

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
