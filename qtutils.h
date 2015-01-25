#ifndef QTUTILS_H
#define QTUTILS_H

#include <QFont>
#include <QMatrix4x4>
#include <QVector4D>
#include <string>

namespace QTUTILS{
    enum Font {
        bold
    };
    QFont getFont();
    static QMatrix4x4 buildTransformationMatrix(float p_scale, float p_x_translation, float p_y_translation,float p_z_translation, int p_x_rotation, int p_y_rotation,
                                         int p_z_rotation);
    static std::string toString(QVector4D& p_matrix);
}

#endif
