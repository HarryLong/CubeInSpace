#include "grid.h"
#include "constants.h"

#define HALF_GRID_COUNT 1500
#define GRID_SEPERATION 5
/********
 * AXES *
 ********/
Axes::Axes() : Asset(true, glm::vec4(.8f, .8f, .8f, 1.f))
{
    // X axis
    m_verticies.push_back(-HALF_GRID_COUNT); m_verticies.push_back(0); m_verticies.push_back(0); // Point 1
    m_verticies.push_back(HALF_GRID_COUNT); m_verticies.push_back(0); m_verticies.push_back(0); // Point 2


    // Z axis
    m_verticies.push_back(0); m_verticies.push_back(0); m_verticies.push_back(-HALF_GRID_COUNT); // Point 1
    m_verticies.push_back(0); m_verticies.push_back(0); m_verticies.push_back(HALF_GRID_COUNT); // Point 2
}

Axes::~Axes()
{

}

void Axes::render()
{
    if(!initalised())
        initGL();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();

    m_vao_constraints.bind();

    f->glDrawArrays(GL_LINES, 0, (GLuint) m_verticies.size()); CE();

    m_vao_constraints.release();
}

void Axes::initGL()
{
    m_vao_constraints.create();
    m_vbo_constraints.create();
    m_ibo_constraints.create();

    m_vao_constraints.bind(); CE();

    m_vbo_constraints.bind();  CE();
    m_vbo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));

    m_vao_constraints.release(); CE();
    m_vbo_constraints.release(); CE();
    m_ibo_constraints.release(); CE();

    fillBuffers();
}

//----------------------------------------------

/*********
 * PLANE *
 *********/
Plane::Plane() : Asset(true, glm::vec4(.2f, .2f, .2f, 1.f))
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

Plane::~Plane()
{

}


void Plane::render()
{
    if(!initalised())
        initGL();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();

    m_vao_constraints.bind();

    f->glDrawArrays(GL_LINES, 0, (GLuint) m_verticies.size()); CE();

    m_vao_constraints.release();
}

void Plane::initGL()
{
    m_vao_constraints.create();
    m_vbo_constraints.create();
    m_ibo_constraints.create();

    m_vao_constraints.bind(); CE();

    m_vbo_constraints.bind();  CE();
    m_vbo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));

    m_vao_constraints.release(); CE();
    m_vbo_constraints.release(); CE();
    m_ibo_constraints.release(); CE();

    fillBuffers();
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

std::vector<Asset*> Grid::getAssets()
{
    return std::vector<Asset*>{get_axes(), get_plane()};
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



