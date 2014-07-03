#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "sceneimpl.h"

#include <QGLWidget>
#include <QScopedPointer>
#include <QMatrix4x4>

class QSurface;
class QMouseEvent;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget * parent = NULL);

signals:
    void transformationMatrixChanged(QMatrix4x4 mat);
    void selectedObjId(int id);
    void signal_newSceneObject(int objId, float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation);

public slots:
    void setTransformation(float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation);
    void setTransformationMatrix(QMatrix4x4 mat);
    void setSelectedObjId(int id);
    void slot_newSceneObject(int objId, float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation);

protected:
    void initializeGL(); // Override
    void paintGL(); // Override
    void resizeGL(int width, int height); // Override
    QSize minimumSizeHint() const; //Override
    void mousePressEvent(QMouseEvent* event); // Override
    QSize sizeHint() const;//Override

private:
    QScopedPointer<AbstractScene> mScene;
};

#endif
