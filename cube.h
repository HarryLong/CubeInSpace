#ifndef QCUBE_H
#define QCUBE_H

#include "globject.h"
#include <QOpenGLBuffer>

class Cube : public GLObject
{
public:
    Cube();
    virtual void draw();
    virtual void drawSelection();
    virtual void initBuffers();
    virtual void initVAO(QOpenGLShaderProgram * mainShader, QOpenGLShaderProgram * selectionShader);

protected:
    QOpenGLBuffer vertexBuffer;
    QOpenGLBuffer indexBuffer;
};
#endif // QCUBE_H
