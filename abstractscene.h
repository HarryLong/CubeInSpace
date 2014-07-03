#ifndef ABSTRACTSCENE_H
#define ABSTRACTSCENE_H

#include <QScopedPointer>
#include <QOpenGLContext>
#include <QMatrix4x4>


class QMouseEvent;
class GLObject;

class AbstractScene : public QObject
{
    Q_OBJECT

public:
    AbstractScene() {}
    virtual ~AbstractScene() {}
    virtual void initialize() = 0;
    virtual void render() = 0;
    virtual void renderSelection() = 0;
    virtual void resize(int width, int height) = 0;
    virtual void setTransformation(float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation) = 0;
    virtual void mousePressEvent(QMouseEvent* event) = 0;

signals:
    void transformationMatrixChanged(QMatrix4x4 matrix);
    void selectedObId(int id);
    void newSceneObject(int objId, float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation);

protected:
    virtual void initSceneElements() = 0;
    virtual void initShaders() = 0;
    virtual void createSceneElement(GLObject* object, float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation) = 0;

private:
};

#endif
