#include "geom.h"
#include "glm/glm.hpp"

/**
 * @brief Geom::rayPointDist
 * @param start
 * @param direction
 * @param query_point
 * @param tval
 * @param dist
 *
 * Get the closest point on vector starting from point 'start' and going in direction 'direction'.
 */
void Geom::rayPointDist(const glm::vec3 & start, const glm::vec3 & direction, const glm::vec3 & query_point, float &scaler, float &dist)
{
    /*
     * Given a line l starting from point A and going in the direction DIR. The point Pc on this line which is the closest to a chosen point Pq
     * will be the projection of point Pq on the line A-DIR. In other words, the line formed by Pc and Pq is perpendicular to the vector A-DIR.
     *
     * Lets call:
     *   - A the vector from A to Pc
     *   - B the vector from Pc to Pq
     *   - C the vector from A to Pq (here @start_to_query)
     *   - V any vector that goes in the same direction as A (here @direction)
     *
     * The triangle ABC is a right angles triangle, therefore:
     *   || A || = cos(alpha) * || C ||   (1)
     *
     * For any V, we have:
     *   V . C = || V || * || C || * cos(alpha) --> || C || * cos(alpha) = (V . C) / || V || (2)
     *
     * By injecting (2) into (1) we see that:
     *   || A || = (V . C) / || V || (3)  [ i.e the vector from A to Pc is of length (V . C) / || V || ]
     *
     * In 2D, we have V = [Xv, Yc] and A = [Xa, Xb]. As A and V go in the same direction:
     *    A = K * V where K = || A || / || V || (3)
     *
     *  Substituting (2) into (3) we get:
     *    K = (V . C) / || V || / || V ||
     *    K = (V . C) / || V ||^2
     *
     *  Here, K = axis_scaler
     */

    float squared_length (squaredLength(direction));
    if(squared_length == 0.0f) // not a valid line segmen
        dist = -1.0f;
    else
    {
        // get parameter value of closest poin
        glm::vec3 start_to_query(diff(query_point, start));

        // Dot product of 'start_to_query' and 'direction'
        scaler = glm::dot(start_to_query, direction);
        scaler /= squared_length;

        // find closest point on line
        glm::vec3 start_to_closest_point(scaler * direction);
        glm::vec3 closest_point(start + start_to_closest_point);

        glm::vec3 query_to_closest_point(diff(closest_point, query_point));
        dist = length(query_to_closest_point);
    }
}
