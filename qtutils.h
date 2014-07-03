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
    QFont getFont(Font font);
    QMatrix4x4 buildTransformationMatrix(float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation);
    std::string toString(QVector4D& matrix);
}

#endif
