#include "qtutils.h"

QFont QTUTILS::getFont()
{
    QFont ret;
//    switch(font){
//    case bold:
//    default:
//        ret.setWeight(QFont::Bold);
//        break;
//    }
    return ret;
}

QMatrix4x4 QTUTILS::buildTransformationMatrix(float p_scale, float p_x_translation, float p_y_translation,float p_z_translation, int p_x_rotation,
                                              int p_y_rotation, int p_z_rotation)
{
    QMatrix4x4 ret;

    // Scale
    ret.scale(p_scale, p_scale, p_scale);

    // Rotate
    ret.rotate(p_x_rotation, 1,0);
    ret.rotate(p_y_rotation, 0,1);
    ret.rotate(p_z_rotation, 0,0,1);

    // Translate
    ret.translate(p_x_translation, p_y_translation, p_z_translation);

    return ret;
}

std::string QTUTILS::toString(QVector4D& p_matrix)
{
    return (std::string("[") + std::to_string(p_matrix.x()) + "," + std::to_string(p_matrix.y()) + "," +
            std::to_string(p_matrix.z()) + "," + std::to_string(p_matrix.w()) + "]");
}
