#include "gl_drawable.h"
#include <iostream>

GlDrawable::GlDrawable() : m_vao_constraints(), m_vbo_constraints(QOpenGLBuffer::Type::VertexBuffer), m_ibo_constraints(QOpenGLBuffer::Type::IndexBuffer)
{

}

GlDrawable::~GlDrawable()
{
    deleteBuffers();
}

void GlDrawable::fillBuffers()
{
    if(m_vbo_constraints.isCreated())
    {
        m_vbo_constraints.bind();  CE();
        m_vbo_constraints.allocate(&m_verticies[0], sizeof(GLfloat)*m_verticies.size()); CE();
        m_vbo_constraints.release();  CE();
    }

    if(m_ibo_constraints.isCreated())
    {
        m_ibo_constraints.bind(); CE();
        m_ibo_constraints.allocate(&m_indicies[0], sizeof(GLint)*m_indicies.size());
        m_ibo_constraints.release(); CE();
    }
}

bool GlDrawable::initalised()
{
    return m_vao_constraints.isCreated();
}

void GlDrawable::deleteBuffers()
{
    if(m_vao_constraints.isCreated()) m_vao_constraints.destroy();  CE();
    if(m_vbo_constraints.isCreated()) m_vbo_constraints.destroy();  CE();
    if(m_ibo_constraints.isCreated()) m_ibo_constraints.destroy();  CE();
}

void GlDrawable::clear()
{
    if(!m_verticies.empty() || !m_indicies.empty())
    {
        m_verticies.clear();
        m_indicies.clear();
        fillBuffers();
    }
}

// TEST METHODS
void GlDrawable::printInfo()
{
    std::cout << "** SHAPE HOLDER INFO" << std::endl;
    std::cout << "Vertices: " << m_verticies.size() << std::endl;
    std::cout << "Indices: " << m_indicies.size() << std::endl;
    std::cout << "**" << std::endl;
}
