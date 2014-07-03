#include "glwidget.h"

#define FRAGMENT_SHADER_SRC "./shaders/shader.frag"
#define VERTEX_SHADER_SRC "./shaders/shader.vert"

#include <QOpenGLContext>
#include <iostream>
#include <QMouseEvent>

GLWidget::GLWidget(QWidget * parent) : QGLWidget(parent), mScene(new SceneImpl())
{
    connect(mScene.data(), SIGNAL(transformationMatrixChanged(QMatrix4x4)), this, SLOT(setTransformationMatrix(QMatrix4x4)));
    connect(mScene.data(), SIGNAL(newSceneObject(int,float,float,float,float,int,int,int)), this, SLOT(slot_newSceneObject(int,float,float,float,float,int,int,int)));
    connect(mScene.data(), SIGNAL(selectedObId(int)), this, SLOT(setSelectedObjId(int)));
}

void GLWidget::initializeGL() // Override
{
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glEnable(GL_DEPTH_TEST);

    mScene->initialize();
}

void GLWidget::paintGL() // Override
{
    mScene->render();
}

void GLWidget::resizeGL(int width, int height) // Override
{
    mScene->resize(width, height);
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::setTransformation(float scale, float xTranslation, float yTranslation, float zTranslation, int xRotation, int yRotation, int zRotation)
{
    mScene->setTransformation(scale, xTranslation, yTranslation, zTranslation, xRotation, yRotation, zRotation);
    updateGL();
}

void GLWidget::setTransformationMatrix(QMatrix4x4 mat)
{
    emit transformationMatrixChanged(mat);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    mScene->mousePressEvent(event);
}

void GLWidget::setSelectedObjId(int id)
{
    emit selectedObjId(id);
}

void GLWidget::slot_newSceneObject(int objId, float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation)
{
    emit(signal_newSceneObject(objId, scale, xTranslation, yTranslation, zTranslation, xRotation, yRotation, zRotation));
}
