#include "gl_drawable.h"

DrawData GlDrawable::getDrawData() const
{
    DrawData draw_data;

    draw_data.m_vao = m_vao_constraints;
    draw_data.m_index_buffer_size = (int) m_indicies.size();
    draw_data.m_vertex_buffer_size = (int) m_verticies.size();

    return draw_data;
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

// TEST METHODS
void GlDrawable::printInfo()
{
    std::cout << "** SHAPE HOLDER INFO" << std::endl;
    std::cout << "Vertices: " << m_verticies.size() << std::endl;
    std::cout << "Indices: " << m_indicies.size() << std::endl;
    std::cout << "**" << std::endl;
}
