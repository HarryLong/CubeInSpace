#ifndef TERRAIN_NORMALS_H
#define TERRAIN_NORMALS_H

#include "gl_drawable.h"
#include "texture_element.h"
#include <glm/common.hpp>
#include <QOpenGLFramebufferObjectFormat>

class QOpenGLFramebufferObject;
/*******************
 * TERRAIN NORMALS *
 *******************/
class TerrainNormals : public GlDrawable
{
public:
    TerrainNormals();
    ~TerrainNormals();
    bool setTerrainDim(int width, int depth);
    GLenum getTextureUnit() const;
    bool valid() const;
    void setValid( bool valid );

    virtual void render();

    glm::vec3 operator()(int x, int z) const;

protected:
    void initGL();

private:
    void init_fbo();
    void delete_fbo();
    void delete_raw_normals();

    static const GLfloat m_screen_quad[8];
//    GLuint m_normalsTexture;
//    GLuint m_normalsTexUnit;

    QOpenGLFramebufferObject * m_fbo_normal_map; // normal map FBO
    QOpenGLFramebufferObjectFormat m_fbo_format;

    int m_width, m_depth;

    bool m_valid;

    GLfloat * m_normals;
    QImage m_img_normals;
};


#endif // TERRAIN_NORMALS_H
