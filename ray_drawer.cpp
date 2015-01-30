#include "ray_drawer.h"
#include <vector>

#define RAY_COLOR 0.506f, 0.008f, 0.969f, 1.0f

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
    GLsizei stride = sizeof(GLfloat) * 7;
    glEnableVertexAttribArray(0);CE()
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0));CE()
    glEnableVertexAttribArray(1);CE()
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*) (sizeof(GLfloat) * 3));CE()
    return true;
}

void RayDrawer::add(const glm::vec3 &start_point, const glm::vec3 &end_point)
{
    std::cout << "ADDING" << std::endl;
    std::cout << "P1: " << start_point[0] << ", " << start_point[1] << ", " << start_point[2] << std::endl;
    std::cout << "P2: " << end_point[0] << ", " << end_point[1] << ", " << end_point[2] << std::endl;

    m_verticies.push_back((GLfloat) start_point[0]);
    m_verticies.push_back(start_point[1]);
    m_verticies.push_back(start_point[2]);

    insertRayColor(m_verticies, RAY_COLOR);

    m_verticies.push_back(end_point[0]);
    m_verticies.push_back(end_point[1]);
    m_verticies.push_back(end_point[2]);

    insertRayColor(m_verticies, RAY_COLOR);

//    m_verticies.push_back(-10.0f);
//    m_verticies.push_back(10.0f);
//    m_verticies.push_back(0.0f);

//    insertRayColor(m_verticies, RAY_COLOR);

//    m_verticies.push_back(10.0f);
//    m_verticies.push_back(10.0f);
//    m_verticies.push_back(0.0f);

//    insertRayColor(m_verticies, RAY_COLOR);
    std::cout << "ADDED" << std::endl;
}

void RayDrawer::insertRayColor(std::vector<GLfloat> & p_vector, float p_red, float p_green, float p_blue, float p_alpha)
{
    p_vector.push_back(p_red); // R
    p_vector.push_back(p_green); // G
    p_vector.push_back(p_blue); // B
    p_vector.push_back(p_alpha); // A
}
