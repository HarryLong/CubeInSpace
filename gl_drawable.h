#ifndef GL_DRAWABLE_H
#define GL_DRAWABLE_H

#include <vector>
#include "glheader.h"

struct DrawData
{
public:
    GLuint m_vao;             // vertex array object id
    GLuint m_index_buffer_size;    // index buffer size - as required by DrawElements
    GLuint m_vertex_buffer_size;    // index buffer size - as required by DrawElements
};

class GlDrawable{

public:
    GlDrawable() : m_vao_constraints(0), m_vbo_constraints(0), m_ibo_constraints(0) {}
    virtual bool bindBuffers() = 0;
    virtual DrawData getDrawData() const;
    virtual void clearData();

protected:
    virtual void delete_buffers();

    std::vector<GLfloat> m_verticies;   // vertex, texture and normal data
    std::vector<GLint> m_indicies;   // vertex indices for triangles
    GLuint m_vao_constraints;    // openGL handles for various buffers
    GLuint m_vbo_constraints;
    GLuint m_ibo_constraints;

    // Test methods
    void printInfo();
};

#endif //GL_DRAWABLE_H
