#ifndef GLHEADERS_H_
#define GLHEADERS_H_
//#define QT_NO_OPENGL_ES_2
#ifdef _WIN32
#include <GL/glew.h>
#else
#define GL3_PROTOTYPES
#include <gl3.h>
#endif
#include <GL/glu.h>
#include <QDebug>
#include <QString>
#include <iostream>

#ifdef CC_GL_DEBUG_
    static GLenum gl_err = 0;
    #define CE() gl_err = glGetError();\
    if (gl_err != GL_NO_ERROR) {\
        const char* err_str = reinterpret_cast<const char *>(gluErrorString(gl_err));\
        QString errString(err_str);\
        qDebug() << "GL Error:" << errString << "on line" << __LINE__ << "of" << __FILE__;\
    }
    #define DEBUG_MSG std::cout << "Got to here --> File: " << __FILE__ << " | Function: " << __FUNCTION__ << " | Line: " << __LINE__ << std::endl;
#else
    #define CE()
    #define DEBUG_MSG
#endif  // CC_GL_DEBUG_

#define RC(CODE) if(CODE == -1){\
    qDebug() << "Function call failed on line" << __LINE__ << "of" << __FILE__;}

#define ERR_MSG(err) QString(QString(err) + QString(" @ Line ") + QString::number(__LINE__) +  QString(" of ") +  QString(__FILE__))

#endif
