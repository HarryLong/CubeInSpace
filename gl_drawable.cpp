#include "gl_drawable.h"
#include <iostream>

GlDrawable::GlDrawable() : m_vao_constraints(0), m_vbo_constraints(0), m_ibo_constraints(0)
{

}

GlDrawable::~GlDrawable()
{
    delete_buffers();
}

void GlDrawable::delete_buffers()
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

// TEST METHODS
void GlDrawable::printInfo()
{
    std::cout << "** SHAPE HOLDER INFO" << std::endl;
    std::cout << "Vertices: " << m_verticies.size() << std::endl;
    std::cout << "Indices: " << m_indicies.size() << std::endl;
    std::cout << "**" << std::endl;
}
