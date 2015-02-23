#ifndef GEOM_H
#define GEOM_H

#include "glm/vec3.hpp"
#include <math.h>

namespace Geom{
    inline float squaredLength(const glm::vec3 vector)
    {
        float len;

        len = vector[0] * vector[0]; //i
        len += vector[1] * vector[1]; //j
        len +=  vector[2] * vector[2]; //j

        return len;
    }

    inline glm::vec3 affinecombine(const float & c1, glm::vec3& p1, const float c2, glm::vec3& p2)
    {
        float x, y, z;
        x = c1 * p1[0] + c2 * p2[0]; //x
        y = c1 * p1[1]+ c2 * p2[1]; //y
        z = c1 * p1[2] + c2 * p2[2]; //z

        return glm::vec3(x,y,z);
    }

    // returns vector p2 to p1
    inline glm::vec3 diff(const glm::vec3 & p1, const glm::vec3 & p2)
    {
        float i, j, k;

        i = (p1[0] - p2[0]);
        j = (p1[1] - p2[1]);
        k = (p1[2] - p2[2]);

        return glm::vec3(i, j, k);
    }

    inline glm::vec3 scale(glm::vec3 & v, const float & c)
    {
        v[0] = v[0] * c;
        v[1] *= c;
        v[2] *= c;
    }

    inline float length(const glm::vec3 & v)
    {
        float length ((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
        return sqrt(length);
    }
    void rayPointDist(const glm::vec3 & start, const glm::vec3 & direction, const glm::vec3 & query_point, float &scaler, float &dist);
}

#endif // GEOM_H
