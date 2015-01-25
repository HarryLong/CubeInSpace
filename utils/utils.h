#ifndef UTILS_H
#define UTILS_H
#include <glm/glm.hpp>

namespace Utils {
void printMatrix4x4(const glm::mat4x4 & mat);
float convertTranslation (int translation);
float convertScale (int scale);
void normalizeAngle(float& p_angle);
}


#endif
