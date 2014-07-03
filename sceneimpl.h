#ifndef QSCENEIMPL_H
#define QSCENEIMPL_H

#include <QOpenGLBuffer>
#include <vector>
#include <QOpenGLShaderProgram>
#include <stack>
#include <map>

#include "abstractscene.h"

struct SceneObject{
    GLObject* object;
    QMatrix4x4 mtwMatrix;

    SceneObject(GLObject* object, QMatrix4x4 mtwMatrix) : object(object), mtwMatrix(mtwMatrix) {}
    void setMTWMatrix(QMatrix4x4 matrix) { mtwMatrix = matrix; }
    const QMatrix4x4& getMTWatrix() {return mtwMatrix; }
};

class SceneImpl : public AbstractScene
{
public:
    SceneImpl();
    virtual ~SceneImpl();
    virtual void initialize();
    virtual void render();
    virtual void renderSelection();
    virtual void resize(int width, int height);
    void setTransformation(float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation);
    virtual void mousePressEvent(QMouseEvent* event);

    // Debug methods
    void printSceneObjects();

protected:
    virtual void initSceneElements();
    virtual void initShaders();
    virtual void initShader(QOpenGLShaderProgram& program, QOpenGLShader::ShaderType type, const QString& filename );
    virtual void createSceneElement(GLObject* object, float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation);

private:
    QOpenGLShaderProgram shader, selectionShader;
    QMatrix4x4 viewMatrix, projection;
    std::map<int,SceneObject> sceneElements;
    int selectedObjectId;
};

#endif // QCUBE_H
