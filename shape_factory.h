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
    virtual void render() const;

protected:
    virtual bool bindBuffers();

private:
    void init();
    float m_radius;
};

/*********
 * ARROW *
 *********/
class GlArrow : public Asset {
public:
    GlArrow(float length, glm::vec4 color);
    ~GlArrow();
    virtual void render() const;
    static const glm::vec3 _base_orientation;

protected:
    virtual bool bindBuffers();

private:
    void init();
    float m_length;
};

/**********
 * SPHERE *
 **********/
class GlSphere : public Asset {
public:
    GlSphere(float radius, int slices, int stacks, glm::vec4 color);
    virtual ~GlSphere();
    virtual void render() const;

protected:
    virtual bool bindBuffers();

private:
    void init();
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
    virtual void render() const;

protected:
    virtual bool bindBuffers();

private:
    void push_front_face_color();
    void push_top_face_color();
    void push_right_face_color();
    void push_bottom_face_color();
    void push_left_face_color();
    void push_back_face_color();

    void init();
};


/*****************
 * SHAPE FACTORY *
 *****************/
class ShapeFactory {
public:
    static GlCircle getCircle(float radius, glm::vec4 color);
    static GlArrow getArrow(float length, glm::vec4 color );
    static GlSphere getSphere(float radius, int slices, int stacks, glm::vec4 color);
    static GlCube getCube(float size);
};

#endif // SHAPE_FACTORY
