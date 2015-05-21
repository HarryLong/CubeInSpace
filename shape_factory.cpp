#include "shape_factory.h"
#include <glm/glm.hpp>
#include <math.h>

/*****************
 * SHAPE FACTORY *
 *****************/
Circle ShapeFactory::getCircle(float radius)
{
    return Circle(radius);
}

Arrow ShapeFactory::getArrow(float length)
{
    return Arrow(length);
}

/**********
 * CIRCLE *
 **********/
Circle::Circle(float radius) : m_radius(radius)
{
    init();
    bindBuffers();
}

Circle::~Circle()
{

}

void Circle::init()
{
    float increments((2*M_PI) / 100);
    int index(0);
    for(float angle(0); angle < 2*M_PI; angle += increments, index++ )
    {
        float x(cos(angle) * m_radius);
        float z(sin(angle) * m_radius);

        // 3D Vertex coordinate
        m_verticies.push_back(x);
        m_verticies.push_back(1.0f); // Y
        m_verticies.push_back(z);

        m_indicies.push_back(index);
    }
}

bool Circle::bindBuffers()
{
    if(m_indicies.size() > 0)
    {
        glGenVertexArrays(1, &m_vao_constraints); CE();
        glBindVertexArray(m_vao_constraints); CE();

        // set up vertex buffer an copy in data
        glGenBuffers(1, &m_vbo_constraints); CE();
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_constraints); CE();
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_verticies.size(), &m_verticies[0], GL_STATIC_DRAW); CE();

        // enable position attribute
        // Vertex position
        glEnableVertexAttribArray(0); CE();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)(0)); CE();

        // set up index buffer
        glGenBuffers(1, &m_ibo_constraints);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_constraints); CE();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)*m_indicies.size(), &m_indicies[0], GL_STATIC_DRAW); CE();
    }
    return true;
}

/*********
 * ARROW *
 *********/
Arrow::Arrow(float length) : m_length(length)
{
    init();
    bindBuffers();
}

Arrow::~Arrow()
{

}

void Arrow::init()
{
    float head_length(0.1f * m_length);

    // Arrow stick
    {
        // Start point of arrow
        m_verticies.push_back(0);
        m_verticies.push_back(0); // Y
        m_verticies.push_back(0);

        // End point of arrow
        m_verticies.push_back(0);
        m_verticies.push_back(0); // Y
        m_verticies.push_back(m_length);

        m_indicies.push_back(0);
        m_indicies.push_back(1);
    }

    // Arrow head
    {
        float z(m_length - head_length);

        // Side 1
        // End point of arrow
        m_verticies.push_back(-head_length);
        m_verticies.push_back(0); // Y
        m_verticies.push_back(z);

        m_indicies.push_back(1);
        m_indicies.push_back(2);

        // Side 1
        // End point of arrow
        m_verticies.push_back(head_length);
        m_verticies.push_back(0); // Y
        m_verticies.push_back(z);

        m_indicies.push_back(1);
        m_indicies.push_back(3);
    }
}

bool Arrow::bindBuffers()
{
    if(m_indicies.size() > 0)
    {
        glGenVertexArrays(1, &m_vao_constraints); CE();
        glBindVertexArray(m_vao_constraints); CE();

        // set up vertex buffer an copy in data
        glGenBuffers(1, &m_vbo_constraints); CE();
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_constraints); CE();
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_verticies.size(), &m_verticies[0], GL_STATIC_DRAW); CE();

        // enable position attribute
        // Vertex position
        glEnableVertexAttribArray(0); CE();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)(0)); CE();

        // set up index buffer
        glGenBuffers(1, &m_ibo_constraints);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_constraints); CE();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)*m_indicies.size(), &m_indicies[0], GL_STATIC_DRAW); CE();
    }
    return true;
}
