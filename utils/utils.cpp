#include "utils.h"
#include <string>
#include <iostream>

void Utils::printMatrix4x4(const glm::mat4x4 & mat)
{
    static const std::string space("   ");
    std::cout << "|  " << mat[0][0] << space << mat[0][1] << space << mat[0][2] << space << std::endl <<
                     "|  " << mat[1][0] << space << mat[1][1] << space << mat[1][2] << space << std::endl <<
                     "|  " << mat[2][0] << space << mat[2][1] << space << mat[2][2] << space << std::endl;
}

float Utils::convertTranslation (int translation)
{
    return translation/10.f;
}

float Utils::convertScale(int scale)
{
    return scale/10.f;
}

void Utils::normalizeAngle(float& p_angle)
{
    while(p_angle > 360.f)
        p_angle -= 360;
    while(p_angle < -360.f)
        p_angle += 360;
}


