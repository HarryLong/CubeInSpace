#ifndef UTILS_H
#define UTILS_H
#include <glm/glm.hpp>
#include <iostream>


namespace Utils {
void printMatrix4x4(const glm::mat4x4 & mat);
float convertTranslation (int translation);
float convertScale (int scale);
void normalizeAngle(float& p_angle);
float toRadians(const float & degrees_angle);
float toDegrees(const float & radians_angle);

inline void print(const glm::vec4 & vec)
{
    std::cout << "[" << vec[0] << "," << vec[1] << "," << vec[2] << "," << vec[3] << "]";
}


inline void print(const glm::vec3 & vec)
{
    std::cout << "[" << vec[0] << "," << vec[1] << "," << vec[2] << "]";
}

}


#endif
