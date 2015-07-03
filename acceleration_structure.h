#ifndef ACCELERATION_STRUCTURE_H
#define ACCELERATION_STRUCTURE_H

#include <vector>
#include "glm/common.hpp"
#include "terragen_file_manager/terragen_file.h"


/***********************
 * ACCELERATION SPHERE *
 ***********************/
class AccelerationSphere {
public:
    AccelerationSphere(glm::vec3 center, int m_radius);
    ~AccelerationSphere();

    bool intersects(const glm::vec3 & start, const glm::vec3 & direction, float scale) const;

    const glm::vec3 m_center;
    const int m_radius;
};

/*******************************
 * BASE ACCELERATION STRUCTURE *
 *******************************/
class Base
{
public:
    Base();
    virtual ~Base();
    std::vector<AccelerationSphere> get_intersecting_spheres(const glm::vec3 & start, const glm::vec3 & direction, float scale);
    void build(const TerragenFile & terrain);

    inline void clear() { m_spheres.clear(); }

    const std::vector<AccelerationSphere> & getSpheres() const;

private:
    const int step_size;
    const int sphere_radius;
    int n_spheres_x;
    int n_spheres_z;

    std::vector<AccelerationSphere> m_spheres;
};

/***************************************
 * HIERARCHICAL ACCELERATION STRUCTURE *
 ***************************************/
class AccelerationTree {
public:
    AccelerationTree(AccelerationSphere node);
    ~AccelerationTree();

    void addChild(AccelerationTree child);
    bool isLeafNode();
    std::vector<AccelerationTree> & getChildren();
    void clear();
    const AccelerationSphere m_node;
private:
    std::vector<AccelerationTree> m_children;
};

class Hierarchical
{
public:
    Hierarchical();
    virtual ~Hierarchical();
    std::vector<AccelerationSphere> get_intersecting_spheres(const glm::vec3 & start, const glm::vec3 & direction, float scale);
    void build(const TerragenFile & terrain);

    void clear();

    const std::vector<AccelerationSphere> & getSpheres() const;

private:
    const int min_sphere_radius;
    void split(AccelerationTree & tree, const TerragenFile & terrain, int tree_depth = 1);
    AccelerationSphere get_sphere(const TerragenFile & terrain, glm::vec2 & center, int base_radius);
    std::vector<AccelerationSphere> get_intersecting_spheres(AccelerationTree & parent, const glm::vec3 & start, const glm::vec3 & direction, float scale);

    AccelerationTree * m_acceleration_tree;
    std::vector<AccelerationSphere> m_spheres;
};

//----------------------------------------------------------------------

template <class T> class SphericalAccelerationStructure : public T
{
public:
    SphericalAccelerationStructure() : T() {}
    ~SphericalAccelerationStructure() {}
    void build(const TerragenFile & terrain) { T::build(terrain); }
    void clear() { T::clear(); }
    std::vector<AccelerationSphere> get_intersecting_spheres(const glm::vec3 & start, const glm::vec3 & direction, float scale) {
        return T::get_intersecting_spheres(start,direction, scale);
    }
    const std::vector<AccelerationSphere> & getSpheres() const { return T::getSpheres(); }

private:
};

#endif // ACCELERATION_STRUCTURE_H
