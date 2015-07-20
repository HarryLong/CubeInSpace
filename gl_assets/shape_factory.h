#ifndef SHAPE_FACTORY_H
#define SHAPE_FACTORY_H

#include "asset.h"

/**********
 * CIRCLE *
 **********/
class GlCircle : public Asset {
public:
    GlCircle(float radius, glm::vec4 color);
    virtual ~GlCircle();
    virtual void render();

protected:
    void initGL();

private:
    void init_vertex_data();

    float m_radius;
};

/*********
 * ARROW *
 *********/
class GlArrow : public Asset {
public:
    GlArrow(float length, glm::vec4 color);
    ~GlArrow();
    virtual void render();
    static const glm::vec3 _base_orientation;

protected:
    void initGL();

private:
    void init_vertex_data();

    float m_length;
};

/***********************
 * SELECTION RECTANGLE *
 ***********************/
class GLTerrainRect : public Asset
{
public:
    GLTerrainRect();
    ~GLTerrainRect();
    void resize(float w_radius, float d_radius);
    void increment_size(int,int);
    virtual void render();
    int w_radius();
    int d_radius();

protected:
    virtual void initGL();

private:
    int m_w_radius;
    int m_d_radius;
};

/**********
 * SPHERE *
 **********/
class GlSphere : public Asset {
public:
    GlSphere(float radius, int slices, int stacks, glm::vec4 color);
    virtual ~GlSphere();
    virtual void render();

protected:
    void initGL();

private:
    void init_vertex_data();
    void append_sphere_vert(float radius, float lat, float lon);
    void get_stack_height_and_radius(int n_stack, float & radius, float & y);

    float m_radius;
    int m_slices;
    int m_stacks;
};

/**********
 * CUBE *
 **********/
class GlCube : public Asset {
public:
    GlCube(float size);
    virtual ~GlCube();
    virtual void render();

protected:
    void initGL();

private:
    void init_vertex_data();
    void push_front_face_color();
    void push_top_face_color();
    void push_right_face_color();
    void push_bottom_face_color();
    void push_left_face_color();
    void push_back_face_color();

};

/*****************
 * SHAPE FACTORY *
 *****************/
class ShapeFactory {
public:
    static GlCircle * getCircle(float radius, glm::vec4 color);
    static GlArrow * getArrow(float length, glm::vec4 color );
    static GLTerrainRect * getTerrainRectangle();
    static GlSphere * getSphere(float radius, int slices, int stacks, glm::vec4 color);
    static GlCube * getCube(float size);
};

#endif // SHAPE_FACTORY
