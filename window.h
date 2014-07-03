#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QScopedPointer>
#include <QComboBox>

#include "controlwidget.h"
#include "statuswidget.h"
#include "matrixwidget.h"

class GLWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

protected:
    QComboBox* createComboBox();

private:
    QScopedPointer<GLWidget> glWidget;
    QScopedPointer<MatrixWidget> matrixWidget;
    QScopedPointer<ControlWidget> controlWidget;
};

#endif
