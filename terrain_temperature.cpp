#include "terrain_temperature.h"


#include "terrain_shade.h"

/***********************
 * TERRAIN TEMPERATURE *
 ***********************/
TerrainTemperature::TerrainTemperature() :
    TextureElement<GLbyte>(QOpenGLTexture::TextureFormat::RG8_SNorm, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Int8)
{

}

TerrainTemperature::~TerrainTemperature()
{

}

///***********************
// * TERRAIN TEMPERATURE *
// ***********************/
//TerrainTemperature::TerrainTemperature(GLenum texture_unit) : m_raw_data(NULL), m_tempTexture(0), m_tempTexUnit(texture_unit), m_valid(false)
//{

//}

//TerrainTemperature::~TerrainTemperature()
//{
//    delete_texture();
//}

//bool TerrainTemperature::isValid() const
//{
//    return m_valid;
//}

//void TerrainTemperature::setTemperatureTexture(GLbyte * data, int width, int depth)
//{
//    // Delete the texture if it already contains shade data
//    delete_texture();
//    if(m_raw_data)
//        free(m_raw_data);

//    // Now update the shade texture with the new values
//    glGenTextures(1, &m_tempTexture); CE();
//    glActiveTexture(m_tempTexUnit); CE();
//    glBindTexture(GL_TEXTURE_2D, m_tempTexture ); CE();

//    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8_SNORM, width, depth, 0, GL_RED, GL_BYTE,  data); CE();
//    // no filtering
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CE();
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CE();
//    // deal with out of array access
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

//    m_raw_data = data;
//    m_width = width;
//    m_depth = depth;
//    m_valid = true;
//}

//GLenum TerrainTemperature::getTextureUnit() const
//{
//    return m_tempTexUnit;
//}

//void TerrainTemperature::invalidate()
//{
//    m_valid = false;
//}

//void TerrainTemperature::delete_texture()
//{
//    if(m_tempTexture != 0)
//    {
//        glDeleteTextures(1, &m_tempTexture);  CE();
//        m_tempTexture = 0;
//    }
//}

//GLbyte TerrainTemperature::operator()(int x, int z) const
//{
//    int index((z*m_width)+x);
//    return m_raw_data[index];
//}
