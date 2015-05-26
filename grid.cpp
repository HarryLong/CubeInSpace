#include "grid.h"
#include "view_manager.h"
#include "constants.h"

#define GRID_SEPERATION 5
#define GRID_COUNT 1500

Grid::Grid() : Asset(true, glm::vec4(.2f, .2f, .2f, 1.f))
{
    init();
    bindBuffers();
}

void Grid::init()
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

bool Grid::bindBuffers()
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
    glGenBuffers(1, &m_vbo_constraints);  CE()
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_constraints);  CE()

    int size (sizeof(GLfloat) * (m_verticies.size()));
    std::cout << "Buffering " << size << " vertices" << std::endl;
    glBufferData(GL_ARRAY_BUFFER, size, &m_verticies[0], GL_STATIC_DRAW);  CE()

    // enable position attribute;
    glEnableVertexAttribArray(0);CE()
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));CE()

    // Unbinding
    glBindVertexArray(0);CE()

    return true;
}

void Grid::render() const
{
    glBindVertexArray(m_vao_constraints); CE(); // Bind VAO

    glDrawArrays(GL_LINES, 0, (GLuint) m_verticies.size()); CE();

    glBindVertexArray(0); CE(); // Unbind VAO
}
