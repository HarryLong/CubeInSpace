#include "grid_manager.h"
#include "view_manager.h"
#include "constants.h"

#define GRID_SEPERATION 5
#define GRID_COUNT 1500

GridHolder::GridHolder() : m_binded(false)
{
    genGrid();
}

void GridHolder::genGrid()
{
    // The grid
    // Along the z axis
    for(int z = -GRID_COUNT; z < GRID_COUNT; z += GRID_SEPERATION)
    {
        // POINT 1
        m_verticies.push_back(-GRID_COUNT); // x
        m_verticies.push_back(0); // y
        m_verticies.push_back(z); //z

        // POINT 2
        m_verticies.push_back(GRID_COUNT); // x
        m_verticies.push_back(0); // y
        m_verticies.push_back(z); //z
    }

    // Along the x axis
    for(int x = -GRID_COUNT; x < GRID_COUNT; x += GRID_SEPERATION)
    {
        // POINT 1
        m_verticies.push_back(x); // x
        m_verticies.push_back(0); // y
        m_verticies.push_back(-GRID_COUNT); //z

        // POINT 2
        m_verticies.push_back(x); // x
        m_verticies.push_back(0); // y
        m_verticies.push_back(GRID_COUNT); //z
    }
}

bool GridHolder::bindBuffers()
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

    m_binded = true;
    return true;
}


