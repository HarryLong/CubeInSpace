#include "grid_manager.h"
#include "view_manager.h"
#include "constants.h"

#define GRID_SEPERATION 1
#define GRID_COUNT 5000

GridHolder::GridHolder()
{
    genGrid();
    bindBuffers();
}

void GridHolder::genGrid()
{
    // First draw the axies
    // The x axis
    {
        // X Positive
        //POINT 1
        m_verticies.push_back(0); // x
        m_verticies.push_back(0); // y
        m_verticies.push_back(0); //z
        insertGridColor(m_verticies, POSITIVE_AXIS_COLOR);
        // POINT 2
        m_verticies.push_back(GRID_COUNT); // x
        m_verticies.push_back(0); // y
        m_verticies.push_back(0); //z
        insertGridColor(m_verticies, POSITIVE_AXIS_COLOR);

        // X Negative
        m_verticies.push_back(0); // x
        m_verticies.push_back(0); // y
        m_verticies.push_back(0); //z
        insertGridColor(m_verticies, NEGATIVE_AXIS_COLOR);
        // POINT 2
        m_verticies.push_back(-GRID_COUNT); // x
        m_verticies.push_back(0); // y
        m_verticies.push_back(0); //z
        insertGridColor(m_verticies, NEGATIVE_AXIS_COLOR);
    }

    // The y axis
    {
        // X Positive
        //POINT 1
        m_verticies.push_back(0); // x
        m_verticies.push_back(0); // y
        m_verticies.push_back(0); //z
        insertGridColor(m_verticies, POSITIVE_AXIS_COLOR);
        // POINT 2
        m_verticies.push_back(0); // x
        m_verticies.push_back(GRID_COUNT); // y
        m_verticies.push_back(0); //z
        insertGridColor(m_verticies, POSITIVE_AXIS_COLOR);

        // X Negative
        m_verticies.push_back(0); // x
        m_verticies.push_back(0); // y
        m_verticies.push_back(0); //z
        insertGridColor(m_verticies, NEGATIVE_AXIS_COLOR);
        // POINT 2
        m_verticies.push_back(0); // x
        m_verticies.push_back(-GRID_COUNT); // y
        m_verticies.push_back(0); //z
        insertGridColor(m_verticies, NEGATIVE_AXIS_COLOR);
    }

    // The z axis
    {
        // X Positive
        //POINT 1
        m_verticies.push_back(0); // x
        m_verticies.push_back(0); // y
        m_verticies.push_back(0); //z
        insertGridColor(m_verticies, POSITIVE_AXIS_COLOR);
        // POINT 2
        m_verticies.push_back(0); // x
        m_verticies.push_back(0); // y
        m_verticies.push_back(GRID_COUNT); //z
        insertGridColor(m_verticies, POSITIVE_AXIS_COLOR);

        // X Negative
        m_verticies.push_back(0); // x
        m_verticies.push_back(0); // y
        m_verticies.push_back(0); //z
        insertGridColor(m_verticies, NEGATIVE_AXIS_COLOR);
        // POINT 2
        m_verticies.push_back(0); // x
        m_verticies.push_back(0); // y
        m_verticies.push_back(-GRID_COUNT); //z
        insertGridColor(m_verticies, NEGATIVE_AXIS_COLOR);
    }

    // The grid
    // Along the z axis
    for(int z = -GRID_COUNT; z < GRID_COUNT; z += GRID_SEPERATION)
    {
        if(z != 0)
        {
            // POINT 1
            m_verticies.push_back(-GRID_COUNT); // x
            m_verticies.push_back(0); // y
            m_verticies.push_back(z); //z
            insertGridColor(m_verticies, BASE_GRID_COLOR);

            // POINT 2
            m_verticies.push_back(GRID_COUNT); // x
            m_verticies.push_back(0); // y
            m_verticies.push_back(z); //z
            insertGridColor(m_verticies, BASE_GRID_COLOR);
        }
    }

    // Along the x axis
    for(int x = -GRID_COUNT; x < GRID_COUNT; x += GRID_SEPERATION)
    {
        if(x != 0)
        {
            // POINT 1
            m_verticies.push_back(x); // x
            m_verticies.push_back(0); // y
            m_verticies.push_back(-GRID_COUNT); //z
            insertGridColor(m_verticies, BASE_GRID_COLOR);

            // POINT 2
            m_verticies.push_back(x); // x
            m_verticies.push_back(0); // y
            m_verticies.push_back(GRID_COUNT); //z
            insertGridColor(m_verticies, BASE_GRID_COLOR);
        }
    }

    for(int i = 0; i < m_verticies.size()/7; i++) // Each 7 elements reference an index --> Point[x,y,z] & Color[R,G,B,A]
        m_indicies.push_back(i);
}

bool GridHolder::bindBuffers()
{
    if(m_indicies.size() > 0)
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

        // ibo
        glGenBuffers(1, &m_ibo_constraints);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_constraints);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)*m_indicies.size(), &m_indicies[0], GL_STATIC_DRAW);CE()

        // enable position attribute;
        GLsizei stride = sizeof(GLfloat) * 7;
        glEnableVertexAttribArray(0);CE()
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0));CE()
        glEnableVertexAttribArray(1);CE()
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*) (sizeof(GLfloat) * 3));CE()

        return true;
    }
    else
    {
        return false;
    }
}

void GridHolder::insertGridColor(std::vector<GLfloat> & p_vector, float p_red, float p_green, float p_blue, float p_alpha)
{
    p_vector.push_back(p_red); // R
    p_vector.push_back(p_green); // G
    p_vector.push_back(p_blue); // B
    p_vector.push_back(p_alpha); // A
}


