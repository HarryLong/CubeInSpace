#ifndef GEOM_H
#define GEOM_H

#include "glm/glm.hpp"
#include <math.h>
#include <vector>
#include <gl.h>
#include <QString>
#include <QDebug>

namespace Geom{
    inline float squaredLength(const glm::vec3 & vector)
    {
        float len;

        len = vector[0] * vector[0]; //i
        len += vector[1] * vector[1]; //j
        len +=  vector[2] * vector[2]; //j

        return len;
    }

    inline float squaredLength(const glm::vec2 & vector)
    {
        float len;

        len = vector[0] * vector[0]; //i
        len += vector[1] * vector[1]; //j

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

    inline glm::vec2 affinecombine(const float & c1, glm::vec2& p1, const float c2, glm::vec2& p2)
    {
        float x, y;
        x = c1 * p1[0] + c2 * p2[0]; //x
        y = c1 * p1[1] + c2 * p2[1]; //y

        return glm::vec2(x,y);
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

    inline glm::vec3 scale(const glm::vec3 & v, const float & c)
    {
        float i, j, k;

        i = v[0] * c;
        j = v[1] * c;
        k = v[2] * c;

        return glm::vec3(i, j, k);
    }

    inline glm::vec2 scale(const glm::vec2 & v, const float & c)
    {
        float i, j;

        i = v[0] * c;
        j = v[1] * c;

        return glm::vec2(i, j);
    }

    inline float length(const glm::vec3 & v)
    {
        float length ((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
        return sqrt(length);
    }

    inline void normalizeDegrees(float& p_angle)
    {
        if(p_angle < 0)
            p_angle += 360;

        while(p_angle > 360.f)
            p_angle -= 360;            
    }

    inline void normalizeRadians(float& p_angle)
    {
        float tmp(p_angle);

        while(p_angle < 0)
            p_angle += 2*M_PI;

        while(p_angle > 2*M_PI)
            p_angle -= 2*M_PI;

        if(tmp != p_angle)
        {

            qCritical() << "Unnormalized: " << tmp;
            qCritical() << "Normalized: " << p_angle;
        }
    }

    inline float toRadians(const float & degrees_angle)
    {
        return degrees_angle / (180/M_PI);
    }

    inline float toDegrees(const float & radians_angle)
    {
        return radians_angle * (180/M_PI);
    }

    QString toString(const glm::mat4x4 & mat);
    void rayPointDist(const glm::vec3 & start, const glm::vec3 & direction, const glm::vec3 & query_point, float &scaler, float &dist);
    bool rayPlaneIntersection(const float & plane_height, const glm::vec3 & ray_start, const glm::vec3 & ray_direction, glm::vec3 & intersection);
}

#endif // GEOM_H
