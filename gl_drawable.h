#ifndef GL_DRAWABLE_H
#define GL_DRAWABLE_H

#include <vector>
#include "glheader.h"

class GlDrawable{

public:
    GlDrawable();
    virtual ~GlDrawable();
    virtual bool bindBuffers() = 0;
    virtual void render() const = 0;

    bool defaultArrayBufferBinding();
    bool defaultElementBufferBinding();

    virtual void clear();
    virtual void deleteBuffers();

    std::vector<GLfloat> m_verticies;   // vertex, texture and normal data
    std::vector<GLint> m_indicies;   // vertex indices for triangles
    GLuint m_vao_constraints;    // openGL handles for various buffers
    GLuint m_vbo_constraints;
    GLuint m_ibo_constraints;
protected:

    // Test methods
    void printInfo();
};

#endif //GL_DRAWABLE_H
