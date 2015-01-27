#include "gl_drawable.h"

DrawData GlDrawable::getDrawData() const
{
    DrawData draw_data;

    draw_data.m_vao = m_vao_constraints;
    draw_data.m_index_buffer_size = (int) m_indicies.size();

    return draw_data;
}

// TEST METHODS
void GlDrawable::printInfo()
{
    std::cout << "** SHAPE HOLDER INFO" << std::endl;
    std::cout << "Vertices: " << m_verticies.size() << std::endl;
    std::cout << "Indices: " << m_indicies.size() << std::endl;
    std::cout << "**" << std::endl;
}
