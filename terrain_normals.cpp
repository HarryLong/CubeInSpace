#include "terrain_normals.h"
#include <iostream>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLFramebufferObject>

/*******************
 * TERRAIN NORMALS *
 *******************/
const GLfloat TerrainNormals::m_screen_quad[8] = {-1.0f, -1.0f,
                                                   1.0f, -1.0f,
                                                   1.0f, 1.0f,
                                                  -1.0f, 1.0f };
TerrainNormals::TerrainNormals() : m_valid(false),
    m_fbo_normal_map(NULL), m_width(0), m_depth(0), m_normals(NULL)
{
//    m_fbo_format.setAttachment(QOpenGLFramebufferObject::Attachment::NoAttachment);
    m_fbo_format.setInternalTextureFormat(GL_RGB16F);

    for(GLfloat coordinate : m_screen_quad)
    {
        m_verticies.push_back(coordinate);
    }
}

TerrainNormals::~TerrainNormals()
{
    delete_fbo();
    delete_raw_normals();
}

void TerrainNormals::delete_fbo()
{
    if(m_fbo_normal_map)
        delete m_fbo_normal_map;
}

void TerrainNormals::init_fbo()
{
    delete_fbo();
    m_fbo_normal_map = new QOpenGLFramebufferObject(m_width, m_depth, m_fbo_format);
}

glm::vec3 TerrainNormals::operator()(int x, int z) const
{
    int start_idx(((z*m_width)+x)*3);
    qCritical() << "Normal: [" << m_normals[start_idx] << ", " << m_normals[start_idx+1]
                 << ", " << m_normals[start_idx+2];

//    QRgb pixel(m_img_normals.pixel(x, z));
//    qCritical() << "Normal: " << qRed(pixel) << ", " << qGreen(pixel)
//                    << ", " << qBlue(pixel);
//    return glm::vec3(qRed(pixel), qGreen(pixel), qBlue(pixel));

    return glm::vec3(m_normals[start_idx], m_normals[start_idx+1], m_normals[start_idx+2]);
}

void TerrainNormals::initGL()
{
    m_vao_constraints.create();
    m_vbo_constraints.create();
    m_ibo_constraints.create();

    m_vao_constraints.bind(); CE();

    m_vbo_constraints.bind();  CE();
    m_vbo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (void*)(0)); CE();

    m_vao_constraints.release(); CE();
    m_vbo_constraints.release(); CE();
    m_ibo_constraints.release(); CE();

    fillBuffers();
}

bool TerrainNormals::setTerrainDim(int width, int depth)
{
    if( m_width != width || m_depth != depth )
    {
        m_width = width;
        m_depth = depth;

        init_fbo();
//        m_fbo_normal_map->bind(); CE();
//        f->glDrawBuffer(GL_COLOR_ATTACHMENT0);  CE(); // "1" is the size of DrawBuffers
//        m_fbo_normal_map->release();

//        m_fbo_normal_map->bind();


        // Now the framebuffer
//        QGLFramebufferObjectFormat format;
//        format.setInternalTextureFormat(GL_RGBA16F);
//        m_fbo_normal_map = new QGLFramebufferObject(QSize(m_width, m_depth), format);
//        m_fbo_normal_map->bind();

        // configure FBO
//        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_normalsTexture, 0); CE();

        // Set the list of draw buffers.
//        GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
//        f->glDrawBuffers(1, drawBuffers);  CE(); // "1" is the size of DrawBuffers

        // Always check that our framebuffer is ok
//        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//        {
//            std::cerr << "Normal map FBO initialisation failed\n";
//            return false;
//        }

        // unbind FBO

//        m_fbo_normal_map->release();
    }
}

void TerrainNormals::render()
{
    if(!initalised())
        initGL();

    if(m_normals)
        free(m_normals);
    m_normals = (GLfloat*) malloc(sizeof(GLfloat)*m_width*m_depth*3);
    memset(m_normals, 0, sizeof(GLfloat)*m_width*m_depth*3);

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();

    m_fbo_normal_map->bind();
    GLenum color_attachments[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, color_attachments);  CE(); // "1" is the size of DrawBuffers

    // set shader program to normal map gen
    m_vao_constraints.bind(); CE();

    f->glDrawArrays(GL_TRIANGLE_FAN, 0, 4);  CE();

    // Retrieve the data
    f->glBindTexture(GL_TEXTURE_2D, m_fbo_normal_map->texture()); CE();
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_FLOAT, (GLvoid*) m_normals ); CE();

    m_vao_constraints.release();
    m_fbo_normal_map->release();
}

GLenum TerrainNormals::getTextureUnit() const
{
    return m_fbo_normal_map->texture();
}

bool TerrainNormals::valid() const
{
    return m_valid;
}

void TerrainNormals::setValid(bool valid)
{
    m_valid = valid;
}

void TerrainNormals::delete_raw_normals()
{
    if(m_normals)
        free(m_normals);
}
