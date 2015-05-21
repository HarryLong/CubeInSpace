#include "ray_drawer.h"
#include <vector>

RayDrawer::RayDrawer()
{

}

bool RayDrawer::bindBuffers()
{
    if (m_vbo_constraints != 0)
    {
        glDeleteVertexArrays(1, &m_vao_constraints);
        glDeleteBuffers(1, &m_vbo_constraints);
        glDeleteBuffers(1, &m_ibo_constraints);
        m_vao_constraints = 0;
        m_vbo_constraints = 0;
        m_ibo_constraints = 0;
    }

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
    return true;
}

void RayDrawer::add(const glm::vec3 &start_point, const glm::vec3 &end_point)
{
    m_verticies.push_back(start_point[0]);
    m_verticies.push_back(start_point[1]);
    m_verticies.push_back(start_point[2]);

    m_verticies.push_back(end_point[0]);
    m_verticies.push_back(end_point[1]);
    m_verticies.push_back(end_point[2]);

//    m_verticies.push_back(-10.0f);
//    m_verticies.push_back(10.0f);
//    m_verticies.push_back(0.0f);

//    insertRayColor(m_verticies, RAY_COLOR);

//    m_verticies.push_back(10.0f);
//    m_verticies.push_back(10.0f);
//    m_verticies.push_back(0.0f);

//    insertRayColor(m_verticies, RAY_COLOR);
}
