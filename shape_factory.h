#ifndef SHAPE_FACTORY_H
#define SHAPE_FACTORY_H

#include "asset.h"

/**********
 * CIRCLE *
 **********/
class GlCircle : public Asset {
public:
    GlCircle(float radius, bool uniform_color, glm::vec4 color = glm::vec4());
    virtual ~GlCircle();
    virtual bool bindBuffers();    
    virtual void render() const;

private:
    void init();
    float m_radius;
};

/*********
 * ARROW *
 *********/
class GlArrow : public Asset {
public:
    GlArrow(float length, bool uniform_color, glm::vec4 color = glm::vec4());
    ~GlArrow();
    virtual bool bindBuffers();
    virtual void render() const;

private:
    void init();
    float m_length;
};

/**********
 * SPHERE *
 **********/
class GlSphere : public Asset {
public:
    GlSphere(float radius, int slices, int stacks, bool uniform_color, glm::vec4 color = glm::vec4());
    virtual ~GlSphere();
    virtual bool bindBuffers();
    virtual void render() const;

private:
    void init();
    void append_sphere_vert(float radius, float lat, float lon);

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
    virtual bool bindBuffers();
    virtual void render() const;

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
    static GlCircle getCircle(float radius, bool uniform_color, glm::vec4 color = glm::vec4());
    static GlArrow getArrow(float length, bool uniform_color, glm::vec4 color = glm::vec4());
    static GlSphere getSphere(float radius, int slices, int stacks, bool uniform_color, glm::vec4 color = glm::vec4());
    static GlCube getCube(float size);
};

#endif // SHAPE_FACTORY
