#include "shape_factory.h"
#include <glm/glm.hpp>
#include <math.h>

//static GlCircle getCircle(float radius, bool uniform_color, glm::vec4 uniform_color = glm::vec4());
//static GlArrow getArrow(float length, bool uniform_color, glm::vec4 uniform_color = glm::vec4());
//static GlSphere getSphere(float radius, int slices, int stacks, bool uniform_color, glm::vec4 uniform_color = glm::vec4());

/*****************
 * SHAPE FACTORY *
 *****************/
GlCircle ShapeFactory::getCircle(float radius, bool uniform_color, glm::vec4 color)
{
    return GlCircle(radius, uniform_color, color);
}

GlArrow ShapeFactory::getArrow(float length, bool uniform_color, glm::vec4 color)
{
    return GlArrow(length, uniform_color, color);
}

GlSphere ShapeFactory::getSphere(float radius, int slices, int stacks, bool uniform_color, glm::vec4 color)
{
    return GlSphere(radius, slices, stacks, uniform_color, color);
}

GlCube ShapeFactory::getCube(float size)
{
    return GlCube(size);
}


/**********
 * CIRCLE *
 **********/
GlCircle::GlCircle(float radius, bool uniform_color, glm::vec4 color) : Asset(uniform_color, color), m_radius(radius)
{
    init();
    bindBuffers();
}

GlCircle::~GlCircle()
{

}

void GlCircle::init()
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

bool GlCircle::bindBuffers()
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

    // Unbinding
    glBindVertexArray(0);CE()

    return true;
}

void GlCircle::render() const
{
    glBindVertexArray(m_vao_constraints); CE();
    glDrawElements(GL_LINES, m_indicies.size(), GL_UNSIGNED_INT, (void*)(0)); CE();
    glBindVertexArray(0); // Unbind
}

/*********
 * ARROW *
 *********/
GlArrow::GlArrow(float length, bool uniform_color, glm::vec4 color) : Asset(uniform_color, color), m_length(length)
{
    init();
    bindBuffers();
}

GlArrow::~GlArrow()
{

}

void GlArrow::init()
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

bool GlArrow::bindBuffers()
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

    // Unbinding
    glBindVertexArray(0);CE()

    return true;
}

void GlArrow::render() const
{
    glBindVertexArray(m_vao_constraints); CE();
    glDrawElements(GL_LINES, m_indicies.size(), GL_UNSIGNED_INT, (void*)(0)); CE();
    glBindVertexArray(0); // Unbind
}

/**********
 * SPHERE *
 **********/
GlSphere::GlSphere(float radius, int slices, int stacks, bool uniform_color, glm::vec4 color) : Asset(uniform_color, color),
    m_radius(radius), m_slices(slices), m_stacks(stacks)
{
    init();
    bindBuffers();
}

GlSphere::~GlSphere()
{
}

bool GlSphere::bindBuffers()
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

    // Unbinding
    glBindVertexArray(0);CE()

    return true;
}

void GlSphere::init()
{
    int index(0);

    for(int lat(0); lat <= m_stacks; lat++)
    {
        float p_lat(((float)lat) / m_stacks);
        float lat_angle(p_lat * M_PI_2);
        float y(std::sin(lat_angle) * m_radius);

        float stack_radius(std::cos(lat_angle) * m_radius);
        int first_stack_index(index);

        for(int lon(0); lon < m_slices; lon++)
        {
            // Upper Stack
            float angle((((float)lon)/m_slices) * 2 * M_PI);

            float x(std::cos(angle) * stack_radius);
            float z(std::sin(angle) * stack_radius);

            m_verticies.push_back(x); m_verticies.push_back(y); m_verticies.push_back(z);

            m_indicies.push_back(index++);

            // Lower Stack
        }
        m_indicies.push_back(first_stack_index);
    }
}

void GlSphere::render() const
{
    glBindVertexArray(m_vao_constraints); CE();
    glDrawElements(GL_LINES, m_indicies.size(), GL_UNSIGNED_INT, (void*)(0)); CE();
    glBindVertexArray(0); // Unbind
}

/**********
 * CUBE *
 **********/
GlCube::GlCube(float size) : Asset(false, glm::vec4(), glm::mat4x4(), size)
{
    init();
}

GlCube::~GlCube()
{

}

bool GlCube::bindBuffers()
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

    // Unbinding
    glBindVertexArray(0);CE()

    return true;
}

void GlCube::render() const
{
    glBindVertexArray(m_vao_constraints); CE();
    glDrawElements(GL_TRIANGLES, m_indicies.size(), GL_UNSIGNED_INT, (void*)(0)); CE();

    glBindVertexArray(0); // Unbind
}

#define RIGHT_EXTENT .5f
#define LEFT_EXTENT -RIGHT_EXTENT
#define TOP_EXTENT .5f
#define BOTTOM_EXTENT -TOP_EXTENT
#define FRONT_EXTENT -.5f
#define REAR_EXTENT -FRONT_EXTENT
void GlCube::init()
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

    bindBuffers();
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


//void Sphere::append_sphere_vert(float radius, float lat, float lon)
//{
//    float la, lo, x, y, z;
//    glm::vec4 p;
//    glm::vec3 v;

//    la = (M_PI+M_PI)*lat;
//    lo = (M_PI+M_PI)*lon;
//    // this is unoptimized
//    x = cos(lo)*sin(la)*radius;
//    y = sin(lo)*sin(la)*radius;
//    z = cos(la)*radius;

////    // apply transformation
////    p = trm * glm::vec4(x, y, z, 1.0f);
////    v = glm::mat3(trm) * glm::normalize(glm::vec3(x, y, z));

//    verts.push_back(p.x); verts.push_back(p.y); verts.push_back(p.z); // position
////    verts.push_back(0.0f); verts.push_back(0.0f); // texture coordinates
////    verts.push_back(v.x); verts.push_back(v.y); verts.push_back(v.z); // normal
//}

//void ConstraintShape::genSphereVert(float radius, float lat, float lon, glm::mat4x4 trm)
//{


//}

//void ConstraintShape::genSphere(float radius, int slices, int stacks, glm::mat4x4 trm)
//{
//    int lat, lon, base;
//    float plat, plon;

//    // doesn't produce very evenly sized triangles, tend to cluster at poles
//    base = int(verts.size()) / 8;
//    for(lat = 0; lat <= stacks; lat++)
//    {
//        for(lon = 0; lon < slices; lon++)
//        {
//            plat = (float) lat / (float) stacks;
//            plon = (float) lon / (float) slices;
//            genSphereVert(radius, plat, plon, trm);

//            if(lat > 0)
//            {
//                if(lon < slices-1)
//                {
//                    indices.push_back(base-slices+lon); indices.push_back(base-slices+lon+1); indices.push_back(base+lon);
//                    indices.push_back(base-slices+lon+1); indices.push_back(base+lon+1); indices.push_back(base+lon);
//                }
//                else // wrap
//                {
//                    indices.push_back(base-slices+lon); indices.push_back(base-slices); indices.push_back(base+lon);
//                    indices.push_back(base-slices); indices.push_back(base); indices.push_back(base+lon);
//                }
//            }
//        }
//        base += slices;
//    }
//}
