#include "window.h"
#include "glwidget.h"

#include <QtWidgets>

Window::Window() : glWidget(new GLWidget(this)), controlWidget(new ControlWidget(this)), matrixWidget(new MatrixWidget(this))
{
    /* BETWEEN THE CONTROLLER AND THE GL WIDGET */
    connect(controlWidget.data(), SIGNAL(signal_transformationChanged(float,float,float,float,int,int,int)),
            glWidget.data(),SLOT(setTransformation(float,float,float,float,int,int,int)));

    /* BETWEEN THE GL WIDGET AND THE CONTROLLER */
    connect(glWidget.data(), SIGNAL(selectedObjId(int)), controlWidget.data(), SLOT(selectController(int)));
    connect(glWidget.data(), SIGNAL(signal_newSceneObject(int,float,float,float,float,int,int,int)), controlWidget.data(),
            SLOT(newSceneObj(int,float,float,float,float,int,int,int)));

    /* BETWEEN THE GL WIDGET AND THE MATRIX WIDGET */
    connect(glWidget.data(), SIGNAL(transformationMatrixChanged(QMatrix4x4)), matrixWidget.data(), SLOT(setMatrix(QMatrix4x4)));

    QGridLayout * mainLayout = new QGridLayout;
    mainLayout->addWidget(glWidget.data(),0,0,3,3);
    mainLayout->addWidget(controlWidget.data(),3,0,1,2);
    mainLayout->addWidget(matrixWidget.data(),3,2,1,1,Qt::AlignCenter);

    setLayout(mainLayout);
    setWindowTitle("CUBES IN SPACE");
}
