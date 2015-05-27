#include "gl_drawable.h"
#include <iostream>

GlDrawable::GlDrawable() : m_vao_constraints(0), m_vbo_constraints(0), m_ibo_constraints(0)
{

}

GlDrawable::~GlDrawable()
{
    deleteBuffers();
}

void GlDrawable::deleteBuffers()
{
    if (m_vbo_constraints != 0) glDeleteBuffers(1, &m_vbo_constraints);  CE();
    if (m_ibo_constraints != 0) glDeleteBuffers(1, &m_ibo_constraints);  CE();
    if (m_vao_constraints != 0) glDeleteVertexArrays(1, &m_vao_constraints);  CE();

    m_vao_constraints = 0;
    m_vbo_constraints = 0;
    m_ibo_constraints = 0;
}

void GlDrawable::clear()
{
    m_verticies.clear();
    m_indicies.clear();
}

bool GlDrawable::defaultArrayBufferBinding()
{
    deleteBuffers();

    // vao
    glGenVertexArrays(1, &m_vao_constraints); CE()
    glBindVertexArray(m_vao_constraints);CE()

    // vbo
    // set up vertex buffer and copy in data
    glGenBuffers(1, &m_vbo_constraints);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_constraints);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_verticies.size(), &m_verticies[0], GL_STATIC_DRAW);

    // enable position attribute;
    glEnableVertexAttribArray(0);CE()
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));CE()

    // Unbinding
    glBindVertexArray(0);CE()

    return true;

}

bool GlDrawable::defaultElementBufferBinding()
{
    deleteBuffers();

    glGenVertexArrays(1, &m_vao_constraints); CE();
    glBindVertexArray(m_vao_constraints); CE();

    // set up vertex buffer an copy in data
    glGenBuffers(1, &m_vbo_constraints); CE();
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_constraints); CE();
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_verticies.size(), &m_verticies[0], GL_STATIC_DRAW); CE();

    // enable position attribute
    // Vertex position
    glEnableVertexAttribArray(0); CE();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)(0)); CE();

    // set up index buffer
    glGenBuffers(1, &m_ibo_constraints);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_constraints); CE();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)*m_indicies.size(), &m_indicies[0], GL_STATIC_DRAW); CE();

    // Unbinding
    glBindVertexArray(0);CE()

    return true;
}

// TEST METHODS
void GlDrawable::printInfo()
{
    std::cout << "** SHAPE HOLDER INFO" << std::endl;
    std::cout << "Vertices: " << m_verticies.size() << std::endl;
    std::cout << "Indices: " << m_indicies.size() << std::endl;
    std::cout << "**" << std::endl;
}
