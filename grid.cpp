#include "grid.h"
#include "view_manager.h"
#include "constants.h"

#define HALF_GRID_COUNT 1500
#define GRID_SEPERATION 5
/********
 * AXES *
 ********/
Axes::Axes() : Asset(true, glm::vec4(.8f, .8f, .8f, 1.f))
{
    init();
    bindBuffers();
}

Axes::~Axes()
{

}

bool Axes::bindBuffers()
{
    return GlDrawable::defaultArrayBufferBinding();
}

void Axes::render() const
{
    glBindVertexArray(m_vao_constraints); CE(); // Bind VAO

    glDrawArrays(GL_LINES, 0, (GLuint) m_verticies.size()); CE();

    glBindVertexArray(0); CE(); // Unbind VAO
}

void Axes::init()
{
    // X axis
    m_verticies.push_back(-HALF_GRID_COUNT); m_verticies.push_back(0); m_verticies.push_back(0); // Point 1
    m_verticies.push_back(HALF_GRID_COUNT); m_verticies.push_back(0); m_verticies.push_back(0); // Point 2


    // Z axis
    m_verticies.push_back(0); m_verticies.push_back(0); m_verticies.push_back(-HALF_GRID_COUNT); // Point 1
    m_verticies.push_back(0); m_verticies.push_back(0); m_verticies.push_back(HALF_GRID_COUNT); // Point 2
}

//----------------------------------------------

/*********
 * PLANE *
 *********/
Plane::Plane() : Asset(true, glm::vec4(.2f, .2f, .2f, 1.f))
{
    init();
    bindBuffers();
}

Plane::~Plane()
{

}

bool Plane::bindBuffers()
{
    return GlDrawable::defaultArrayBufferBinding();
}

void Plane::render() const
{
    glBindVertexArray(m_vao_constraints); CE(); // Bind VAO

    glDrawArrays(GL_LINES, 0, (GLuint) m_verticies.size()); CE();

    glBindVertexArray(0); CE(); // Unbind VAO
}

void Plane::init()
{
    // Parrallel to Z axis
    for(int z (GRID_SEPERATION) ; z < HALF_GRID_COUNT; z += GRID_SEPERATION)
    {
        // Positive side of z-axis
        m_verticies.push_back(-HALF_GRID_COUNT); m_verticies.push_back(0); m_verticies.push_back(z); // Point 1
        m_verticies.push_back(HALF_GRID_COUNT); m_verticies.push_back(0); m_verticies.push_back(z); // Point 2

        // Negative side of z-axis
        m_verticies.push_back(-HALF_GRID_COUNT); m_verticies.push_back(0); m_verticies.push_back(-z); // Point 1
        m_verticies.push_back(HALF_GRID_COUNT); m_verticies.push_back(0); m_verticies.push_back(-z); // Point 2
    }

    // Parrallel to x axis
    for(int x (GRID_SEPERATION); x < HALF_GRID_COUNT; x += GRID_SEPERATION)
    {
        // Positive side of x-axis
        m_verticies.push_back(x); m_verticies.push_back(0); m_verticies.push_back(-HALF_GRID_COUNT); // Point 1
        m_verticies.push_back(x); m_verticies.push_back(0); m_verticies.push_back(HALF_GRID_COUNT); // Point 2

        // Negative side of x-axis
        m_verticies.push_back(-x); m_verticies.push_back(0); m_verticies.push_back(-HALF_GRID_COUNT); // Point 1
        m_verticies.push_back(-x); m_verticies.push_back(0); m_verticies.push_back(HALF_GRID_COUNT); // Point 2
    }
}

//----------------------------------------------

/********
 * GRID *
 ********/
Grid::Grid() : m_axes(NULL), m_plane(NULL)
{

}

Grid::~Grid()
{
    if(m_axes)
        delete m_axes;
    if(m_plane)
        delete m_plane;
}

std::vector<const Asset*> Grid::getAssets()
{
    return std::vector<const Asset*>{get_axes(), get_plane()};
}

Axes * Grid::get_axes()
{
    if(!m_axes)
        m_axes = new Axes();

    return m_axes;
}

Plane * Grid::get_plane()
{
    if(!m_plane)
        m_plane = new Plane();

    return m_plane;
}



