#ifndef GL_DRAWABLE_H
#define GL_DRAWABLE_H

#include <vector>
#include "glheader.h"
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

class GlDrawable{
public:
    GlDrawable();
    virtual ~GlDrawable();

    virtual void render() = 0;

    virtual void clear();
    virtual void deleteBuffers();

    bool initalised();

protected:
    virtual void initGL() = 0;
    virtual void fillBuffers();

    // Test methods
    void printInfo();

    std::vector<GLfloat> m_verticies;   // vertex, texture and normal data
    std::vector<GLuint> m_indicies;   // vertex indices for triangles
    QOpenGLVertexArrayObject m_vao_constraints;    // openGL handles for various buffers
    QOpenGLBuffer m_vbo_constraints;
    QOpenGLBuffer m_ibo_constraints;
};

#endif //GL_DRAWABLE_H
