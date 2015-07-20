#include "soil_infiltration.h"
#include <QOpenGLFunctions>
#include <algorithm>

SoilInfiltration::SoilInfiltration() :
    TextureElement<GLuint>(QOpenGLTexture::TextureFormat::R32U, QOpenGLTexture::PixelFormat::Red_Integer, QOpenGLTexture::PixelType::UInt32)
{

}

SoilInfiltration::~SoilInfiltration()
{

}

void SoilInfiltration::update(const glm::vec2 & center, int radius, int terrain_width, int terrain_depth, int infiltration_rate)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    glm::vec2 top_left(std::max(.0f, center.x-radius), std::max(.0f, center.y-radius));
    glm::vec2 bottom_right(std::min((int)center.x+radius, terrain_width-1), std::min((int)center.y+radius, terrain_depth-1));
    int width(bottom_right.x-top_left.x+1);
    int height(bottom_right.y-top_left.y+1);

    int sz(width*height*sizeof(GLuint));
    GLuint * data = (GLuint*) std::malloc(sz);

#pragma omp parallel for
    for(int x = 0; x < width; x++)
    {
        for(int z = 0; z < height; z++)
        {
            data[x*height+z] = (GLuint) infiltration_rate;
        }
    }

    // First bind the texture
    bind();
    f->glTexSubImage2D(GL_TEXTURE_2D,
                       0,
                       top_left.x,
                       top_left.y,
                       width,
                       height,
                       GL_RED_INTEGER,
                       GL_UNSIGNED_INT,
                       data); CE();
}
