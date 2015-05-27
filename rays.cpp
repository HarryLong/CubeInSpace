#include "rays.h"
#include <vector>

RayDrawer::RayDrawer() : Asset(true, glm::vec4(.294f, .0f, .51f, 1.f))
{

}

RayDrawer::~RayDrawer()
{

}

bool RayDrawer::bindBuffers()
{
    return GlDrawable::defaultArrayBufferBinding();
}

void RayDrawer::render() const
{
    glBindVertexArray(m_vao_constraints); CE();

    glDrawArrays(GL_LINES, 0, m_verticies.size()); CE();

    glBindVertexArray(0); // Unbind
}

void RayDrawer::add(const glm::vec3 &start_point, const glm::vec3 &end_point)
{
    m_verticies.push_back(start_point[0]);
    m_verticies.push_back(start_point[1]);
    m_verticies.push_back(start_point[2]);

    m_verticies.push_back(end_point[0]);
    m_verticies.push_back(end_point[1]);
    m_verticies.push_back(end_point[2]);

    bindBuffers();
}
