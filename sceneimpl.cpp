#include "sceneimpl.h"
#include "glheader.h"
#include "cube.h"
#include "globject.h"
#include "utils/utils.h"

#include <qtutils.h>
#include <QMouseEvent>

#define FRAGMENT_SHADER_LOCATION "/home/harry/qt_workspace/CubeInSpace/shaders/shader.frag"
#define VERTEX_SHADER_LOCATION "/home/harry/qt_workspace/CubeInSpace/shaders/shader.vert"
#define SELECTION_FRAGMENT_SHADER_LOCATION "/home/harry/qt_workspace/CubeInSpace/shaders/selection_shader.frag"
#define SELECTION_VERTEX_SHADER_LOCATION "/home/harry/qt_workspace/CubeInSpace/shaders/selection_shader.vert"

#define UNIFORM_MTW_MATRIX "modelToWorldMatrix"
#define UNIFORM_VIEW_MATRIX "viewMatrix"
#define UNIFORM_PROJECTION_MATRIX "projectionMatrix"
#define UNIFORM_COLOR_CODE "colorCode"
#define UNIFORM_BRIGHTNESS_INCREMENT "brightnessIncrement"

#define NONE 0

SceneImpl::SceneImpl() : shader(), selectionShader(), sceneElements(), projection(), viewMatrix(), selectedObjectId(NONE)
{

}

SceneImpl::~SceneImpl()
{
    for(std::map<int,SceneObject>::iterator it = sceneElements.begin(); it != sceneElements.end(); ++it)
    {
        delete it->second.object;
    }
}

void SceneImpl::initialize()
{
    initShaders();
    initSceneElements();
}

void SceneImpl::initShaders()
{
    /* INITIALIZE THE MAIN SHADER */
    initShader(shader, QOpenGLShader::Vertex, VERTEX_SHADER_LOCATION); // Vertex shader
    initShader(shader, QOpenGLShader::Fragment, FRAGMENT_SHADER_LOCATION); // Fragment shader
    if (!shader.link())
    {
        qCritical() << "error";
    }

    /* INITIALIZE THE SELECTION SHADER */
    initShader(selectionShader, QOpenGLShader::Vertex, SELECTION_VERTEX_SHADER_LOCATION); // Vertex shader
    initShader(selectionShader, QOpenGLShader::Fragment, SELECTION_FRAGMENT_SHADER_LOCATION); // Fragment shader
    if (!selectionShader.link())
    {
        qCritical() << "error";
    }
}

void SceneImpl::initShader(QOpenGLShaderProgram& shader, QOpenGLShader::ShaderType type, const QString& filename )
{
    if (!shader.addShaderFromSourceFile(type, filename))
    {
        qCritical() << "error";
    }
}


void SceneImpl::initSceneElements()
{
    QMatrix4x4 mtwMat;
    int i(1);

    // First Cube is centered horizontally (no transformation
    createSceneElement(new Cube(),1,0,0,0,0,0,0);

    // Second cube is shifted left from center cube
    createSceneElement(new Cube(),1,-.8f,0,0,0,0,0);

    // Third cube is shifted right from center cube
    createSceneElement(new Cube(),1,.8f,0,0,0,0,0);

    // Fourth cube is up from center cube
    createSceneElement(new Cube(),1,0,.8f,0,0,0,0);

    // Fifth cube is shifter down from center cube
    createSceneElement(new Cube(),1,0,-.8f,0,0,0,0);

    printSceneObjects();
}

void SceneImpl::createSceneElement(GLObject* object, float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation)
{
    static int uniqueElementId(1);
//    QMatrix4x4 mtwMatrix;
    QMatrix4x4 mtwMatrix(QTUTILS::buildTransformationMatrix(scale, xTranslation, yTranslation, zTranslation, xRotation, yRotation, zRotation));
    object->initVAO(&shader, &selectionShader);
    sceneElements.insert(std::pair<int,SceneObject>(uniqueElementId, SceneObject(object, mtwMatrix)));

    emit newSceneObject(uniqueElementId, scale, xTranslation, yTranslation, zTranslation, xRotation, yRotation, zRotation);

    uniqueElementId++;
}

void SceneImpl::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    if(!shader.bind())
    {
        qCritical() << "error";
    }
    shader.setUniformValue(UNIFORM_VIEW_MATRIX, viewMatrix);

    for(std::map<int,SceneObject>::iterator it = sceneElements.begin(); it != sceneElements.end(); ++it)
    {
        shader.setUniformValue(UNIFORM_MTW_MATRIX, it->second.getMTWatrix());
        shader.setUniformValue(UNIFORM_BRIGHTNESS_INCREMENT, selectedObjectId == it->first ? .4f : .0f);
        it->second.object->draw();
    }
}

void SceneImpl::renderSelection()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    if(!selectionShader.bind())
    {
        qCritical() << "error";
    }
    selectionShader.setUniformValue(UNIFORM_VIEW_MATRIX, viewMatrix);

    for(std::map<int,SceneObject>::iterator it = sceneElements.begin(); it != sceneElements.end(); ++it)
    {
        selectionShader.setUniformValue(UNIFORM_MTW_MATRIX, it->second.getMTWatrix());
        selectionShader.setUniformValue(UNIFORM_COLOR_CODE, it->first);
        it->second.object->drawSelection();
    }
}

void SceneImpl::resize(int w, int h)
{
    // Set OpenGL viewport to cover whole widget
    glViewport(0, 0, w, h);

    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1., zFar = 100.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);

    shader.bind();
    shader.setUniformValue(UNIFORM_PROJECTION_MATRIX, projection);
    shader.release();

    selectionShader.bind();
    selectionShader.setUniformValue(UNIFORM_PROJECTION_MATRIX, projection);
    selectionShader.release();
}

void SceneImpl::setTransformation(float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation)
{
    QMatrix4x4 transformationMatrix = QTUTILS::buildTransformationMatrix(scale, xTranslation, yTranslation, zTranslation, xRotation, yRotation, zRotation);
    if(selectedObjectId == NONE)
    {
        viewMatrix = transformationMatrix;
        emit transformationMatrixChanged(viewMatrix);
    }
    else
    {
        std::map<int,SceneObject>::iterator it;
        if((it = sceneElements.find(selectedObjectId)) != sceneElements.end())
        {
            it->second.setMTWMatrix(transformationMatrix);
        }
    }
}

void SceneImpl::mousePressEvent(QMouseEvent* event)
{
    // First render the selection
    renderSelection();

    // Read depth of pixel
    GLfloat depth;
    glReadPixels(event->x(), event->y(),1,1,GL_DEPTH_COMPONENT,GL_FLOAT, &depth);

    // Get object code
    unsigned char objCode[4];
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glReadPixels(event->x(), viewport[3] - event->y(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &objCode);
    int iObjCode = (int) objCode[0];

    // Set selected cube
    selectedObjectId = iObjCode;
    emit(selectedObId(selectedObjectId));

    std::cout << "******************************CLICK EVENT*************************************" << std::endl;
    std::cout << "Position: [" << event->x() << "," << event->y() << "," << depth << "]" << std::endl;
    std::cout << "Color code: " << iObjCode<< std::endl;
    std::cout << "******************************************************************************" << std::endl;
}

// DEBUG METHODS
void SceneImpl::printSceneObjects()
{
    std::cout << "*********SCENE OBJECTS*********" << std::endl;
    for(std::map<int, SceneObject>::iterator it = sceneElements.begin(); it != sceneElements.end(); ++it)
    {
        std::cout << "-- Object " << it->first << std::endl;
        Utils::printMatrix4x4(it->second.getMTWatrix().constData());
        std::cout << "--" << std::endl << std::endl;
    }
    std::cout << "*****************************" << std::endl;
}

