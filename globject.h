#ifndef GLOBJECT_H
#define GLOBJECT_H

#include "glheader.h"
#include "sceneimpl.h"

#include <QOpenGLVertexArrayObject>

class QOpenGLShaderProgram;

class GLObject
{
public:
    GLObject() {}
    virtual ~GLObject() {}

    virtual void draw() = 0;
    virtual void drawSelection() = 0;
    virtual void initVAO(QOpenGLShaderProgram * mainShader, QOpenGLShaderProgram * selectionShader) = 0;
    virtual void initBuffers() = 0;

protected:
    QOpenGLVertexArrayObject mainVAO, selectionVAO;

private:
};

#endif
