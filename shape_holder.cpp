#include "shape_holder.h"
#include "boost/foreach.hpp"
#include "debug.h"
/* TODO: ALL THIS SHOULD BE CREATED DYNAMICALLY BASED ON PARAMETERS PASSED THROUGH */

#define RIGHT_EXTENT 1.f
#define LEFT_EXTENT -RIGHT_EXTENT
#define TOP_EXTENT 1.f
#define BOTTOM_EXTENT -TOP_EXTENT
#define FRONT_EXTENT -1.f
#define REAR_EXTENT -FRONT_EXTENT

#define GREEN_COLOR .0f, 1.f, .0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.0f, 1.0f, 1.0f
#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREY_COLOR 0.8f, 0.8f, 0.8f, 1.0f
#define BROWN_COLOR 0.5f, 0.5f, 0.0f, 1.0f
#define YELLOW_COLOR 1.f, 1.f, 0.0f, 1.0f

#define NUMBER_OF_VERTICES 24

const GLfloat g_vertex_data[] = {
    /************** VERTEX DATA ************//*** COLOR DATA *****/
    // FRONT FACE  [0,1,2][2,3,0]
    LEFT_EXTENT,     TOP_EXTENT, FRONT_EXTENT,  GREEN_COLOR,
    RIGHT_EXTENT,    TOP_EXTENT, FRONT_EXTENT,  GREEN_COLOR,
    RIGHT_EXTENT, BOTTOM_EXTENT, FRONT_EXTENT,  GREEN_COLOR,
    LEFT_EXTENT,  BOTTOM_EXTENT, FRONT_EXTENT,  GREEN_COLOR,

    // TOP FACE [4,5,6][6,7,4]
    LEFT_EXTENT,      TOP_EXTENT, REAR_EXTENT,  BLUE_COLOR,
    RIGHT_EXTENT,     TOP_EXTENT, REAR_EXTENT,  BLUE_COLOR,
    RIGHT_EXTENT,    TOP_EXTENT, FRONT_EXTENT,  BLUE_COLOR,
    LEFT_EXTENT,     TOP_EXTENT, FRONT_EXTENT,  BLUE_COLOR,

//    // RIGHT_FACE  [8,9,10][10,11,8]
    RIGHT_EXTENT,    TOP_EXTENT, FRONT_EXTENT,  RED_COLOR,
    RIGHT_EXTENT,     TOP_EXTENT, REAR_EXTENT,  RED_COLOR,
    RIGHT_EXTENT,  BOTTOM_EXTENT, REAR_EXTENT,  RED_COLOR,
    RIGHT_EXTENT, BOTTOM_EXTENT, FRONT_EXTENT,  RED_COLOR,

//    // BOTTOM FACE [12,13,14][14,15,12]
    RIGHT_EXTENT, BOTTOM_EXTENT, FRONT_EXTENT,  GREY_COLOR,
    RIGHT_EXTENT,  BOTTOM_EXTENT, REAR_EXTENT,  GREY_COLOR,
    LEFT_EXTENT,   BOTTOM_EXTENT, REAR_EXTENT,  GREY_COLOR,
    LEFT_EXTENT,  BOTTOM_EXTENT, FRONT_EXTENT,  GREY_COLOR,

//    // LEFT FACE [16,17,18][18,19,16]
    LEFT_EXTENT,     TOP_EXTENT,  REAR_EXTENT,  BROWN_COLOR,
    LEFT_EXTENT,    TOP_EXTENT,  FRONT_EXTENT,  BROWN_COLOR,
    LEFT_EXTENT, BOTTOM_EXTENT,  FRONT_EXTENT,  BROWN_COLOR,
    LEFT_EXTENT,  BOTTOM_EXTENT,  REAR_EXTENT,  BROWN_COLOR,

//    // BACK FACE [20,23,22][22,21,20]
    RIGHT_EXTENT,    TOP_EXTENT,  REAR_EXTENT,  YELLOW_COLOR,
    LEFT_EXTENT,    TOP_EXTENT,   REAR_EXTENT,  YELLOW_COLOR,
    LEFT_EXTENT,  BOTTOM_EXTENT,  REAR_EXTENT,  YELLOW_COLOR,
    RIGHT_EXTENT, BOTTOM_EXTENT,  REAR_EXTENT,  YELLOW_COLOR
};

const GLushort g_index_data[] =
{
//     FRONT FACE
    0,1,2,  0,1,2,
    2,3,0,  2,3,0,

    // TOP FACE
    4,5,6,  4,5,6,
    6,7,4,  6,7,4,

    // RIGHT_FACE
    8,9,10,  8,9,10,
    10,11,8, 10,11,8,

    // BOTTOM FACE
    12,13,14,  12,13,14,
    14,15,12,  14,15,12,

    // LEFT FACE
    16,17,18,  16,17,18,
    18,19,16,  18,19,16,

    // BACK FACE
    20,21,22,  20,21,22,
    22,23,20,  22,23,20,
};

CubeDrawable::CubeDrawable()
{
    BOOST_FOREACH(GLfloat v, g_vertex_data)
    {
        m_verticies.push_back(v);
    }
    BOOST_FOREACH(GLushort i, g_index_data)
    {
    m_indicies.push_back(i);
    }
    bindBuffers();
}

bool CubeDrawable::bindBuffers()
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
        glGenBuffers(1, &m_vbo_constraints);CE()
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo_constraints);CE()
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m_verticies.size(), &m_verticies[0], GL_STATIC_DRAW);CE()

        // ibo
        glGenBuffers(1, &m_ibo_constraints);CE()
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo_constraints);CE()
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint)*m_indicies.size(), &m_indicies[0], GL_STATIC_DRAW);CE()

        // enable position attribute
        GLsizei stride = sizeof(GLfloat) * 7;
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * 3));

        glBindVertexArray(0);CE()

        return true;
    }
    else
    {
        PRINT_POSITION()
        return false;
    }
}

/* END TODO */
ShapesHolder::ShapesHolder() : m_cube(NULL)
{
}

ShapesHolder::~ShapesHolder()
{
    delete m_cube;
}

GlDrawable * ShapesHolder::get(Shape shape)
{
    switch(shape)
    {
    case Cube:
        if(!m_cube)
            m_cube = new CubeDrawable;

        return m_cube;
    }
}
