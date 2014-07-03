#include "qtutils.h"

QFont QTUTILS::getFont(Font font)
{
    QFont ret;
    switch(font){
    case bold:
    default:
        ret.setWeight(QFont::Bold);
        break;
    }
    return ret;
}

QMatrix4x4 QTUTILS::buildTransformationMatrix(float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation)
{
    QMatrix4x4 ret;

    // Scale
    ret.scale(scale, scale, scale);

    // Rotate
    ret.rotate(xRotation, 1,0);
    ret.rotate(yRotation, 0,1);
    ret.rotate(zRotation, 0,0,1);

    // Translate
    ret.translate(xTranslation, yTranslation, zTranslation);

    return ret;
}

std::string QTUTILS::toString(QVector4D& matrix)
{
    return (std::string("[") + std::to_string(matrix.x()) + "," + std::to_string(matrix.y()) + "," +
            std::to_string(matrix.z()) + "," + std::to_string(matrix.w()) + "]");
}
