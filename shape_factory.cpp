#include "shape_factory.h"
#include <glm/glm.hpp>
#include <math.h>

/*****************
 * SHAPE FACTORY *
 *****************/
GlCircle * ShapeFactory::getCircle(float radius, glm::vec4 color)
{
    return new GlCircle(radius, color);
}

GlArrow * ShapeFactory::getArrow(float length, glm::vec4 color)
{
    return new GlArrow(length, color);
}

GLSelectionRect * ShapeFactory::getSelectionRectangle()
{
    return new GLSelectionRect;
}

GlSphere * ShapeFactory::getSphere(float radius, int slices, int stacks, glm::vec4 color)
{
    return new GlSphere(radius, slices, stacks, color);
}

GlCube * ShapeFactory::getCube(float size)
{
    return new GlCube(size);
}

/**********
 * CIRCLE *
 **********/
GlCircle::GlCircle(float radius, glm::vec4 color) : Asset(true, color), m_radius(radius)
{
    init_vertex_data();
}

GlCircle::~GlCircle()
{

}

void GlCircle::init_vertex_data()
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

void GlCircle::initGL()
{
    m_vao_constraints.create();
    m_vbo_constraints.create();
    m_ibo_constraints.create();

    m_vao_constraints.bind(); CE();

    m_vbo_constraints.bind();  CE();
    m_vbo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    m_ibo_constraints.bind(); CE();
    m_ibo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));

    m_vao_constraints.release(); CE();
    m_vbo_constraints.release(); CE();
    m_ibo_constraints.release(); CE();

    fillBuffers();
}

void GlCircle::render()
{
    if(!initalised())
        initGL();

    m_vao_constraints.bind();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();

    f->glDrawElements(GL_LINE_LOOP, m_indicies.size(), GL_UNSIGNED_INT, (void*)(0)); CE();

    m_vao_constraints.release(); // Unbind
}

/*********
 * ARROW *
 *********/
const glm::vec3 GlArrow::_base_orientation = glm::vec3(0,0,1); // Do not change this!
GlArrow::GlArrow(float length, glm::vec4 color) : Asset(true, color), m_length(length)
{
    init_vertex_data();
}

GlArrow::~GlArrow()
{

}

void GlArrow::init_vertex_data()
{
    float head_length(0.1f * m_length);

    // Arrow stick
    {
        // Start point of arrow
        m_verticies.push_back(0);
        m_verticies.push_back(0); // Y
        m_verticies.push_back(0);

        // End point of arrow
        m_verticies.push_back(_base_orientation[0] * m_length);
        m_verticies.push_back(_base_orientation[1] * m_length); // Y
        m_verticies.push_back(_base_orientation[2] * m_length);

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

void GlArrow::initGL()
{
    m_vao_constraints.create();
    m_vbo_constraints.create();
    m_ibo_constraints.create();

    m_vao_constraints.bind(); CE();

    m_vbo_constraints.bind();  CE();
    m_vbo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    m_ibo_constraints.bind(); CE();
    m_ibo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));

    m_vao_constraints.release(); CE();
    m_vbo_constraints.release(); CE();
    m_ibo_constraints.release(); CE();

    fillBuffers();
}

void GlArrow::render()
{
    if(!initalised())
        initGL();

    m_vao_constraints.bind();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();

    f->glDrawElements(GL_LINES, m_indicies.size(), GL_UNSIGNED_INT, (void*)(0)); CE();

    m_vao_constraints.release(); // Unbind
}

/*********************
 * TERRAIN RECTANGLE *
 *********************/
GLSelectionRect::GLSelectionRect() : Asset(true, glm::vec4(0.3, 0.3, 0.3, 0.5))
{

}

GLSelectionRect::~GLSelectionRect()
{

}

void GLSelectionRect::resize(glm::vec3 min, glm::vec3 max, int terrain_width, int terrain_depth)
{
    clear();

    int rect_width(max[0] - min[0]);
    int rect_depth(max[2] - min[2]);

    // Vertices
    for (int z (min[2]); z < max[2]; z++)
    {
        for (int x (min[0]); x < max[0]; x++)
        {
            // 3D Vertex coordinate
            m_verticies.push_back((float) x);
            m_verticies.push_back(.0f); // Y (stored in heightmap texture)
            m_verticies.push_back((float) z);

            // 2D texture coordinate (for the heightmap)
            m_verticies.push_back((float) x / (float) (terrain_width - 1)); // X
            m_verticies.push_back((float) z / (float) (terrain_depth - 1)); // Y
        }
    }

    // Indices
    for (int z(0); z < rect_depth - 1; z++)
    {
        if (z > 0) // Degenerate begin: repeat first vertex
            m_indicies.push_back((GLuint) (z * rect_width));

        for (int x = 0; x < rect_width; x++)
        {
            // One part of the strip
            m_indicies.push_back((GLuint) ((z * rect_width) + x));
            m_indicies.push_back((GLuint) (((z + 1) * rect_width) + x));
        }

        if (z <  rect_depth - 2)   // Degenerate end: repeat last vertex
            m_indicies.push_back((GLuint) (((z + 1) * rect_width) + (rect_width - 1)));
    }
    fillBuffers();
}

void GLSelectionRect::initGL()
{
    m_vao_constraints.create();
    m_vbo_constraints.create();
    m_ibo_constraints.create();

    m_vao_constraints.bind(); CE();

    m_vbo_constraints.bind();  CE();
    m_vbo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    m_ibo_constraints.bind(); CE();
    m_ibo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0); CE();
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(0)); CE();
    // Texture coordinate
    f->glEnableVertexAttribArray(1); CE();
    const int sz = 3*sizeof(GLfloat);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(sz)); CE();

    m_vao_constraints.release(); CE();
    m_vbo_constraints.release(); CE();
    m_ibo_constraints.release(); CE();

    fillBuffers();
}

void GLSelectionRect::render()
{
    if(!initalised())
        initGL();

    // enable position attribute;
    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_vao_constraints.bind();
    f->glDrawElements(GL_TRIANGLE_STRIP, m_indicies.size(), GL_UNSIGNED_INT, (void*)(0)); CE();
    m_vao_constraints.release();

    f->glDisable(GL_BLEND);
}

/**********
 * SPHERE *
 **********/
GlSphere::GlSphere(float radius, int slices, int stacks, glm::vec4 color) : Asset(true, color),
    m_radius(radius), m_slices(slices), m_stacks(stacks)
{
    init_vertex_data();
}

GlSphere::~GlSphere()
{

}

void GlSphere::init_vertex_data()
{
    int index(0);
    float r0, r1;
    float x0, x1;
    float y0, y1;
    float z0, z1;
    int lower_stack_index, upper_stack_index;
    for(int stack_idx(0); stack_idx < m_stacks; stack_idx++)
    {
        get_stack_height_and_radius(stack_idx, r0, y0);
        get_stack_height_and_radius(stack_idx+1, r1, y1);

        for(int slice(0); slice <= m_slices; slice++)
        {
            float angle((((float)slice)/m_slices) * 2 * M_PI);
            /***********************
             * NORTHERN HEMISPHERE *
             ***********************/
            {
                x0 = std::cos(angle) * r0;
                x1 = std::cos(angle) * r1;

                z0 = std::sin(angle) * r0;
                z1 = std::sin(angle) * r1;

                lower_stack_index = index++;
                upper_stack_index = index++;

                m_verticies.push_back(x0); m_verticies.push_back(y0); m_verticies.push_back(z0); // Lower stack
                m_verticies.push_back(x1); m_verticies.push_back(y1); m_verticies.push_back(z1); // Upper stack

                if(slice > 0) // Last two points of quad
                {
                    m_indicies.push_back(upper_stack_index);
                    m_indicies.push_back(lower_stack_index);
                }
                if(slice < m_slices) // First two points of quad
                {
                    m_indicies.push_back(lower_stack_index);
                    m_indicies.push_back(upper_stack_index);
                }
            }
        }
    }
}

void GlSphere::get_stack_height_and_radius(int n_stack, float & radius, float & y)
{
    float p_stack(((float)n_stack)/m_stacks);
    float stack_angle(-M_2_PI + (p_stack * M_PI));

    y = std::sin(stack_angle) * m_radius;
    radius = std::cos(stack_angle) * m_radius;
}

void GlSphere::initGL()
{
    m_vao_constraints.create();
    m_vbo_constraints.create();
    m_ibo_constraints.create();

    m_vao_constraints.bind(); CE();

    m_vbo_constraints.bind();  CE();
    m_vbo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    m_ibo_constraints.bind(); CE();
    m_ibo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));

    m_vao_constraints.release(); CE();
    m_vbo_constraints.release(); CE();
    m_ibo_constraints.release(); CE();

    fillBuffers();
}

void GlSphere::render()
{
    if(!initalised())
        initGL();

    m_vao_constraints.bind();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();

    f->glDrawElements(GL_TRIANGLES, m_indicies.size(), GL_UNSIGNED_INT, (void*)(0)); CE();

    m_vao_constraints.release(); // Unbind
}

/**********
 * CUBE *
 **********/
GlCube::GlCube(float size) : Asset(false, glm::vec4(), glm::mat4x4(), size)
{
    init_vertex_data();
}

GlCube::~GlCube()
{

}

void GlCube::initGL()
{
    m_vao_constraints.create();
    m_vbo_constraints.create();
    m_ibo_constraints.create();

    m_vao_constraints.bind(); CE();

    m_vbo_constraints.bind();  CE();
    m_vbo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    m_ibo_constraints.bind(); CE();
    m_ibo_constraints.setUsagePattern(QOpenGLBuffer::UsagePattern::StaticDraw);  CE();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();
    GLsizei stride = sizeof(GLfloat) * 7;
    f->glEnableVertexAttribArray(0); CE();
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0)); CE();
    f->glEnableVertexAttribArray(1); CE();
    f->glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat) * 3)); CE();

    m_vao_constraints.release(); CE();
    m_vbo_constraints.release(); CE();
    m_ibo_constraints.release(); CE();

    fillBuffers();
}

#include <iostream>
void GlCube::render()
{
    if(!initalised())
        initGL();

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();

    m_vao_constraints.bind(); CE();

    f->glDrawElements(GL_TRIANGLES, m_indicies.size(), GL_UNSIGNED_INT, (void*)(0)); CE();

    m_vao_constraints.release(); CE(); // Unbind
}

#define RIGHT_EXTENT .5f
#define LEFT_EXTENT -RIGHT_EXTENT
#define TOP_EXTENT .5f
#define BOTTOM_EXTENT -TOP_EXTENT
#define FRONT_EXTENT -.5f
#define REAR_EXTENT -FRONT_EXTENT
void GlCube::init_vertex_data()
{
    //Front face
    {
        int start_index(m_verticies.size()/7);

        m_verticies.push_back(LEFT_EXTENT); m_verticies.push_back(TOP_EXTENT);m_verticies.push_back(FRONT_EXTENT); push_front_face_color();
        m_verticies.push_back(RIGHT_EXTENT); m_verticies.push_back(TOP_EXTENT);m_verticies.push_back(FRONT_EXTENT); push_front_face_color();
        m_verticies.push_back(RIGHT_EXTENT); m_verticies.push_back(BOTTOM_EXTENT);m_verticies.push_back(FRONT_EXTENT); push_front_face_color();
        m_verticies.push_back(LEFT_EXTENT); m_verticies.push_back(BOTTOM_EXTENT);m_verticies.push_back(FRONT_EXTENT); push_front_face_color();

        m_indicies.push_back(start_index); m_indicies.push_back(start_index+1); m_indicies.push_back(start_index+2);
        m_indicies.push_back(start_index+2); m_indicies.push_back(start_index+3); m_indicies.push_back(start_index);
    }

    // Right face
    {
        int start_index(m_verticies.size()/7);
        m_verticies.push_back(RIGHT_EXTENT); m_verticies.push_back(TOP_EXTENT);m_verticies.push_back(FRONT_EXTENT); push_right_face_color();
        m_verticies.push_back(RIGHT_EXTENT); m_verticies.push_back(TOP_EXTENT);m_verticies.push_back(REAR_EXTENT); push_right_face_color();
        m_verticies.push_back(RIGHT_EXTENT); m_verticies.push_back(BOTTOM_EXTENT);m_verticies.push_back(REAR_EXTENT); push_right_face_color();
        m_verticies.push_back(RIGHT_EXTENT); m_verticies.push_back(BOTTOM_EXTENT);m_verticies.push_back(FRONT_EXTENT); push_right_face_color();

        m_indicies.push_back(start_index); m_indicies.push_back(start_index+1); m_indicies.push_back(start_index+2);
        m_indicies.push_back(start_index+2); m_indicies.push_back(start_index+3); m_indicies.push_back(start_index);
    }

    // Bottom face
    {
        int start_index(m_verticies.size()/7);

        m_verticies.push_back(RIGHT_EXTENT); m_verticies.push_back(BOTTOM_EXTENT);m_verticies.push_back(FRONT_EXTENT); push_bottom_face_color();
        m_verticies.push_back(RIGHT_EXTENT); m_verticies.push_back(BOTTOM_EXTENT);m_verticies.push_back(REAR_EXTENT); push_bottom_face_color();
        m_verticies.push_back(LEFT_EXTENT); m_verticies.push_back(BOTTOM_EXTENT);m_verticies.push_back(REAR_EXTENT); push_bottom_face_color();
        m_verticies.push_back(LEFT_EXTENT); m_verticies.push_back(BOTTOM_EXTENT);m_verticies.push_back(FRONT_EXTENT); push_bottom_face_color();

        m_indicies.push_back(start_index); m_indicies.push_back(start_index+1); m_indicies.push_back(start_index+2);
        m_indicies.push_back(start_index+2); m_indicies.push_back(start_index+3); m_indicies.push_back(start_index);
    }

    // Left face
    {
        int start_index(m_verticies.size()/7);

        m_verticies.push_back(LEFT_EXTENT); m_verticies.push_back(TOP_EXTENT);m_verticies.push_back(REAR_EXTENT); push_left_face_color();
        m_verticies.push_back(LEFT_EXTENT); m_verticies.push_back(TOP_EXTENT);m_verticies.push_back(FRONT_EXTENT); push_left_face_color();
        m_verticies.push_back(LEFT_EXTENT); m_verticies.push_back(BOTTOM_EXTENT);m_verticies.push_back(FRONT_EXTENT); push_left_face_color();
        m_verticies.push_back(LEFT_EXTENT); m_verticies.push_back(BOTTOM_EXTENT);m_verticies.push_back(REAR_EXTENT); push_left_face_color();

        m_indicies.push_back(start_index); m_indicies.push_back(start_index+1); m_indicies.push_back(start_index+2);
        m_indicies.push_back(start_index+2); m_indicies.push_back(start_index+3); m_indicies.push_back(start_index);
    }

    // Top face
    {
        int start_index(m_verticies.size()/7);

        m_verticies.push_back(LEFT_EXTENT); m_verticies.push_back(TOP_EXTENT);m_verticies.push_back(REAR_EXTENT); push_top_face_color();
        m_verticies.push_back(RIGHT_EXTENT); m_verticies.push_back(TOP_EXTENT);m_verticies.push_back(REAR_EXTENT); push_top_face_color();
        m_verticies.push_back(RIGHT_EXTENT); m_verticies.push_back(TOP_EXTENT);m_verticies.push_back(FRONT_EXTENT); push_top_face_color();
        m_verticies.push_back(LEFT_EXTENT); m_verticies.push_back(TOP_EXTENT);m_verticies.push_back(FRONT_EXTENT); push_top_face_color();

        m_indicies.push_back(start_index); m_indicies.push_back(start_index+1); m_indicies.push_back(start_index+2);
        m_indicies.push_back(start_index+2); m_indicies.push_back(start_index+3); m_indicies.push_back(start_index);
    }

    // Back face
    {
        int start_index(m_verticies.size()/7);

        m_verticies.push_back(RIGHT_EXTENT); m_verticies.push_back(TOP_EXTENT);m_verticies.push_back(REAR_EXTENT); push_back_face_color();
        m_verticies.push_back(LEFT_EXTENT); m_verticies.push_back(TOP_EXTENT);m_verticies.push_back(REAR_EXTENT); push_back_face_color();
        m_verticies.push_back(LEFT_EXTENT); m_verticies.push_back(BOTTOM_EXTENT);m_verticies.push_back(REAR_EXTENT); push_back_face_color();
        m_verticies.push_back(RIGHT_EXTENT); m_verticies.push_back(BOTTOM_EXTENT);m_verticies.push_back(REAR_EXTENT); push_back_face_color();

        m_indicies.push_back(start_index); m_indicies.push_back(start_index+1); m_indicies.push_back(start_index+2);
        m_indicies.push_back(start_index+2); m_indicies.push_back(start_index+3); m_indicies.push_back(start_index);
    }
}

void GlCube::push_front_face_color()  // GREEN
{
    m_verticies.push_back(.0f); m_verticies.push_back(1.0f); m_verticies.push_back(.0f); m_verticies.push_back(1.0f);
}

void GlCube::push_top_face_color() // BLUE
{
    m_verticies.push_back(.0f); m_verticies.push_back(.0f); m_verticies.push_back(1.0f); m_verticies.push_back(1.0f);
}

void GlCube::push_right_face_color() // RED
{
    m_verticies.push_back(1.0f); m_verticies.push_back(.0f); m_verticies.push_back(.0f); m_verticies.push_back(1.0f);
}

void GlCube::push_bottom_face_color() // GREY
{
    m_verticies.push_back(.8f); m_verticies.push_back(.8f); m_verticies.push_back(.8f); m_verticies.push_back(1.0f);
}

void GlCube::push_left_face_color() //BROWN
{
    m_verticies.push_back(.5f); m_verticies.push_back(.5f); m_verticies.push_back(.0f); m_verticies.push_back(1.0f);
}

void GlCube::push_back_face_color() // YELLOW
{
    m_verticies.push_back(1.0f); m_verticies.push_back(1.0f); m_verticies.push_back(.0f); m_verticies.push_back(1.0f);
}
