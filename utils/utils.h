#ifndef UTILS_H
#define UTILS_H

namespace Utils {
void printMatrix4x4(const float* mat);
float convertTranslation (int translation);
float convertScale (int scale);
void normalizeAngle(int& angle);
}


#endif
