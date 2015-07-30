#include "drawable_terrain.h"
#include <cfloat>

/********************
 * DRAWABLE TERRAIN *
 ********************/
DrawableTerrain::DrawableTerrain() :
    TextureElement<GLfloat>(QOpenGLTexture::TextureFormat::R32F, QOpenGLTexture::PixelFormat::Red, QOpenGLTexture::PixelType::Float32)
{

}

DrawableTerrain::~DrawableTerrain()
{

}

#include <iostream>
void DrawableTerrain::render()
{
    if(!initalised())
        initGL();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();

    m_vao_constraints.bind();

    f->glDrawElements(GL_TRIANGLE_STRIP, m_indicies.size(), GL_UNSIGNED_INT, (void*)(0)); CE();

    m_vao_constraints.release();
}

void DrawableTerrain::initGL()
{
    m_vao_constraints.create(); CE();
    m_vbo_constraints.create(); CE();
    m_ibo_constraints.create(); CE();

    m_vao_constraints.bind(); CE();

    m_vbo_constraints.bind();  CE();
    m_vbo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    m_ibo_constraints.bind(); CE();
    m_ibo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0); CE();
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0)); CE();
    // Texture coordinate
//    f->glEnableVertexAttribArray(1); CE();
//    const int sz = 3*sizeof(GLfloat);
//    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(sz)); CE();

    m_vao_constraints.release(); CE();
    m_vbo_constraints.release(); CE();
    m_ibo_constraints.release(); CE();

    fillBuffers();
}

bool DrawableTerrain::prepareTerrainGeometry(const TerragenFile & terragen_file)
{
    clear();

    for (float z (.0f); z < terragen_file.m_header_data.depth ; z++)
    {
        for (float x( 0 ); x < terragen_file.m_header_data.width; x++)
        {
            // 3D Vertex coordinate
            m_verticies.push_back(x);/*- (m_header_data.width/2 * m_header_data.dynamic_scale )*/ // X
            m_verticies.push_back(.0f); // Y (stored in heightmap texture)
            m_verticies.push_back(z);/*- (m_header_data.depth/2 * m_header_data.dynamic_scale )*/ // Z

//            // 2D texture coordinate
//            m_verticies.push_back((float) x / (float) (terragen_file.m_header_data.width)); // X
//            m_verticies.push_back((float) z / (float) (terragen_file.m_header_data.depth)); // Y
        }
    }

    // Indices
    m_indicies.clear();
    for (int z = 0; z < terragen_file.m_header_data.width - 1; z++)
    {
        if (z > 0) // Degenerate begin: repeat first vertex
            m_indicies.push_back((GLuint) (z * terragen_file.m_header_data.width));

        for (int x = 0; x < terragen_file.m_header_data.width; x++)
        {
            // One part of the strip
            m_indicies.push_back((GLuint) ((z * terragen_file.m_header_data.width) + x));
            m_indicies.push_back((GLuint) (((z + 1) * terragen_file.m_header_data.width) + x));
        }

        if (z <  terragen_file.m_header_data.depth - 2)   // Degenerate end: repeat last vertex
            m_indicies.push_back((GLuint) (((z + 1) * terragen_file.m_header_data.width) + (terragen_file.m_header_data.width - 1)));
    }

    fillBuffers();
    refreshHeightmapTexture(terragen_file);
}

void DrawableTerrain::refreshHeightmapTexture(const TerragenFile & terragen_file)
{
    setData(terragen_file.m_height_data, terragen_file.m_header_data.width, terragen_file.m_header_data.depth);
//    pushToGPU();
}
