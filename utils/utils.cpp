#include "utils.h"
#include <string>
#include <iostream>

void Utils::printMatrix4x4(const float* mat)
{
    std::string space("   ");
    for(int i = 0; i < 4; ++i)
    {
        int startIdx = i*4;
        std::cout << "|  " << mat[startIdx] << space << mat[startIdx+1] << space << mat[startIdx+2] << space << mat[startIdx+3] << " |" << std::endl;
    }
}

float Utils::convertTranslation (int translation)
{
    return translation/10.f;
}

float Utils::convertScale(int scale)
{
    return scale/10.f;
}

void Utils::normalizeAngle(int& angle)
{
    while(angle < 0)
        angle += 360;

    angle %= 360;
}


