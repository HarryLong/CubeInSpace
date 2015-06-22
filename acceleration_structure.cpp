#include "acceleration_structure.h"
#include "geom.h"
#include <mutex>

/***********************
 * ACCELERATION SPHERE *
 ***********************/
AccelerationSphere::AccelerationSphere(glm::vec3 center, int radius) : m_center(center), m_radius(radius)
{

}

AccelerationSphere::~AccelerationSphere()
{

}

bool AccelerationSphere::intersects(const glm::vec3 & start, const glm::vec3 & direction, float scale) const
{
    float scaler, distance;
    Geom::rayPointDist(start, direction, m_center*scale, scaler, distance) ;
    return distance < m_radius*scale;
}

/*******************************
 * BASE ACCELERATION STRUCTURE *
 *******************************/
#define SPHERE_ACCELERATION_STEP_SIZE 50
Base::Base() :
    step_size(SPHERE_ACCELERATION_STEP_SIZE),
    sphere_radius(std::sqrt(2*std::pow(std::ceil(step_size/2.0f),2))),
    n_spheres_x(0),
    n_spheres_z(0)
{

}

Base::~Base()
{

}

std::vector<AccelerationSphere> Base::get_intersecting_spheres(const glm::vec3 & start, const glm::vec3 & direction, float scale)
{
    std::mutex container_mutex;
    std::vector<AccelerationSphere> intersecting_spheres;

    #pragma omp parallel for
    for(int i = 0; i < m_spheres.size(); i++)
    {
        AccelerationSphere sphere (m_spheres.at(i));
        if(sphere.intersects(start, direction, scale))
        {
            container_mutex.lock();
            intersecting_spheres.push_back(sphere);
            container_mutex.unlock();
        }
    }

    return intersecting_spheres;
}

void Base::build(const TerragenFile & terrain)
{
    clear();

    n_spheres_x = (terrain.m_header_data.width-1) / step_size + 1;
    n_spheres_z = (terrain.m_header_data.depth-1) / step_size + 1;

    for(int center_x = step_size; center_x < terrain.m_header_data.width; center_x += step_size )
    {
        for(int center_z = step_size; center_z < terrain.m_header_data.depth ; center_z += step_size)
        {
            int x_min(center_x-sphere_radius);
            int z_min(center_z-sphere_radius);

            int x_max(x_min + 2 * sphere_radius);
            int z_max(z_min + 2 * sphere_radius);

            glm::vec3 p_min(x_min, terrain(std::max(x_min,0),std::max(z_min,0)), z_min);
            glm::vec3 p_max(x_max, terrain(std::min(terrain.m_header_data.width-1,x_max),std::min(terrain.m_header_data.depth-1,z_max)), z_max);

            glm::vec3 sphere_center(Geom::affinecombine(0.5f, p_min, 0.5f, p_max));

            // Calculate the maximum radius
            float squared_radius(.0f);
            #pragma omp parallel for
            for( int point_in_sphere_x = x_min; point_in_sphere_x < x_max; point_in_sphere_x++ )
                for( int point_in_sphere_z = z_min; point_in_sphere_z < z_max; point_in_sphere_z++)
                {
                    glm::vec3 point_in_sphere(point_in_sphere_x,
                                              terrain(
                                                  std::max(0,std::min(point_in_sphere_x,terrain.m_header_data.width)),
                                                  std::max(0,std::min(point_in_sphere_z,terrain.m_header_data.width))),
                                              point_in_sphere_z);

                    // calculate the distance from the center
                    glm::vec3 center_to_point(Geom::diff(sphere_center, point_in_sphere));

                    float squared_length(Geom::squaredLength(center_to_point));

                    if(squared_length > squared_radius)
                        squared_radius = squared_length;
                }

            // Scale & Add sphere to structure
            float radius(sqrt(squared_radius));

            m_spheres.push_back(AccelerationSphere(sphere_center, radius));
        }
    }
}

const std::vector<AccelerationSphere> & Base::getSpheres() const
{
    return m_spheres;
}

/***************************************
 * HIERARCHICAL ACCELERATION STRUCTURE *
 ***************************************/
AccelerationTree::AccelerationTree(AccelerationSphere node) : m_node(node)
{

}

AccelerationTree::~AccelerationTree()
{

}

void AccelerationTree::addChild(AccelerationTree child)
{
    m_children.push_back(child);
}

bool AccelerationTree::isLeafNode()
{
    return m_children.empty();
}

std::vector<AccelerationTree> & AccelerationTree::getChildren()
{
    return m_children;
}

void AccelerationTree::clear()
{
    m_children.clear();
}

//---------------------------------------------------------------------------------------
#define MIN_SPHERE_RADIUS 5
Hierarchical::Hierarchical() :
    m_acceleration_tree(NULL), min_sphere_radius(MIN_SPHERE_RADIUS)
{

}

Hierarchical::~Hierarchical()
{
    clear();
}

void Hierarchical::clear()
{
    if(m_acceleration_tree)
        delete m_acceleration_tree;
}

std::vector<AccelerationSphere> Hierarchical::get_intersecting_spheres(AccelerationTree & parent, const glm::vec3 & start, const glm::vec3 & direction, float scale)
{
    std::vector<AccelerationSphere> intersecting_spheres;

    if(parent.m_node.intersects(start, direction, scale))
    {
        if(parent.isLeafNode()) // Add it if leaf node, we have reached the end of the tree
            intersecting_spheres.push_back(parent.m_node);
        else
        {
            for(AccelerationTree & child : parent.getChildren())
            {
                std::vector<AccelerationSphere> child_intersecting_spheres(get_intersecting_spheres(child, start, direction, scale));
//                intersecting_spheres.insert(intersecting_spheres.end(), child_intersecting_spheres.begin(), child_intersecting_spheres.end());

                for(AccelerationSphere sphere : child_intersecting_spheres)
                    intersecting_spheres.push_back(sphere);
            }
        }
    }

    return intersecting_spheres;
}

std::vector<AccelerationSphere> Hierarchical::get_intersecting_spheres(const glm::vec3 & start, const glm::vec3 & direction, float scale)
{
    return get_intersecting_spheres(*m_acceleration_tree, start, direction, scale);
}

AccelerationSphere Hierarchical::get_sphere(const TerragenFile & terrain, glm::vec2 & center, int base_radius)
{
    int x_min(center[0]-base_radius);
    int z_min(center[1]-base_radius);

    int x_max(center[0]+base_radius);
    int z_max(center[1]+base_radius);

    glm::vec3 p_min(x_min, terrain(std::max(x_min,0),std::max(z_min,0)), z_min);
    glm::vec3 p_max(x_max, terrain(std::min(terrain.m_header_data.width-1,x_max),std::min(terrain.m_header_data.depth-1,z_max)), z_max);

    glm::vec3 sphere_center(Geom::affinecombine(0.5f, p_min, 0.5f, p_max));

    // Calculate the maximum radius
    float squared_radius(.0f);
    #pragma omp parallel for
    for( int point_in_sphere_x = x_min; point_in_sphere_x < x_max; point_in_sphere_x++ )
        for( int point_in_sphere_z = z_min; point_in_sphere_z < z_max; point_in_sphere_z++)
        {
            glm::vec3 point_in_sphere(point_in_sphere_x,
                                      terrain(
                                          std::max(0,std::min(point_in_sphere_x,terrain.m_header_data.width-1)),
                                          std::max(0,std::min(point_in_sphere_z,terrain.m_header_data.depth-1))),
                                      point_in_sphere_z);

            // calculate the distance from the center
            glm::vec3 center_to_point(Geom::diff(sphere_center, point_in_sphere));

            float squared_length(Geom::squaredLength(center_to_point));

            if(squared_length > squared_radius)
                squared_radius = squared_length;
        }

    // Scale & Add sphere to structure
    float radius(std::sqrt(squared_radius));

    return AccelerationSphere(sphere_center, radius);
}

void Hierarchical::build(const TerragenFile & terrain)
{
    clear();

    // First intialise the root node
    glm::vec2 min(0,0);
    glm::vec2 max(terrain.m_header_data.width, terrain.m_header_data.depth);
    glm::vec2 center(Geom::affinecombine(0.5f, min, 0.5f, max));

    int radius(std::ceil(terrain.m_header_data.width / 2.0f));

    m_acceleration_tree = new AccelerationTree(get_sphere(terrain, center, radius));

    // start the splitting process (recursive)
    split(*m_acceleration_tree, terrain);
}

#include <iostream>
void Hierarchical::split(AccelerationTree &tree, const TerragenFile & terrain, int tree_depth)
{
    glm::vec3 parent_center(tree.m_node.m_center);
    int depth_radius(std::ceil(terrain.m_header_data.width / pow(2, ++tree_depth)));

//    std::cout << "Building tree depth: " << tree_depth << "(Radius:" << depth_radius << ")" << std::endl;

    if(depth_radius > min_sphere_radius)
    {
        // Add the 4 children
        for(int x = -1; x <= 1; x+=2)
        {
            for(int z = -1; z <= 1; z+=2)
            {
                glm::vec2 center(parent_center[0] + x * depth_radius, parent_center[2] + z * depth_radius);
                AccelerationSphere sphere (get_sphere(terrain, center, depth_radius));
                m_spheres.push_back(sphere);
                tree.addChild(sphere);
            }
        }
        for(AccelerationTree & child_tree : tree.getChildren())
        {
            split(child_tree, terrain, tree_depth); // Recursive
        }
    }
}

const std::vector<AccelerationSphere> & Hierarchical::getSpheres() const
{
    return m_spheres;
}
