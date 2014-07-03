#include "cube.h"
#include "utils/utils.h" // TEMP
#include <QOpenGLShaderProgram>

//#define RIGHT_EXTENT 0.8f
//#define LEFT_EXTENT .0f
//#define TOP_EXTENT .8f
//#define BOTTOM_EXTENT .0f
//#define FRONT_EXTENT .8f
//#define REAR_EXTENT .0f

#define RIGHT_EXTENT .2f
#define LEFT_EXTENT -RIGHT_EXTENT
#define TOP_EXTENT .2f
#define BOTTOM_EXTENT -TOP_EXTENT
#define FRONT_EXTENT -3.f
#define REAR_EXTENT -7.f

#define GREEN_COLOR .0f, 1.f, .0f, 1.0f
#define BLUE_COLOR 	0.0f, 0.0f, 1.0f, 1.0f
#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREY_COLOR 0.8f, 0.8f, 0.8f, 1.0f
#define BROWN_COLOR 0.5f, 0.5f, 0.0f, 1.0f
#define YELLOW_COLOR 1.f, 1.f, 0.0f, 1.0f

#define NUMBER_OF_VERTICES 24

const float vertexData[] = {
    // FRONT FACE  [0,1,2][2,3,0]
    LEFT_EXTENT,    TOP_EXTENT,     FRONT_EXTENT, 1.0,//0
    RIGHT_EXTENT,    TOP_EXTENT,     FRONT_EXTENT, 1.0, //1
    RIGHT_EXTENT,    BOTTOM_EXTENT,     FRONT_EXTENT,1.0, //2
    LEFT_EXTENT,    BOTTOM_EXTENT,     FRONT_EXTENT, 1.0, //3

    // TOP FACE [4,5,6][6,7,4]
    LEFT_EXTENT, TOP_EXTENT, REAR_EXTENT, 1.0,// 4
    RIGHT_EXTENT, TOP_EXTENT, REAR_EXTENT, 1.0,// 5
    RIGHT_EXTENT, TOP_EXTENT, FRONT_EXTENT,1.0, // 6
    LEFT_EXTENT, TOP_EXTENT, FRONT_EXTENT,1.0,// 7

//    // RIGHT_FACE  [8,9,10][10,11,8]
    RIGHT_EXTENT, TOP_EXTENT, FRONT_EXTENT, 1.0,// 8
    RIGHT_EXTENT, TOP_EXTENT, REAR_EXTENT, 1.0,// 9
    RIGHT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT, 1.0,// 10
    RIGHT_EXTENT, BOTTOM_EXTENT, FRONT_EXTENT,1.0, // 11

//    // BOTTOM FACE [12,13,14][14,15,12]
    RIGHT_EXTENT, BOTTOM_EXTENT, FRONT_EXTENT, 1.0,// 12
    RIGHT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,1.0, // 13
    LEFT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT,1.0, // 14
    LEFT_EXTENT, BOTTOM_EXTENT, FRONT_EXTENT, 1.0,// 15

//    // LEFT FACE [16,17,18][18,19,16]
    LEFT_EXTENT, TOP_EXTENT, REAR_EXTENT,1.0, // 16
    LEFT_EXTENT, TOP_EXTENT, FRONT_EXTENT, 1.0,// 17
    LEFT_EXTENT, BOTTOM_EXTENT, FRONT_EXTENT, 1.0,// 18
    LEFT_EXTENT, BOTTOM_EXTENT, REAR_EXTENT, 1.0,// 19

//    // BACK FACE [20,23,22][22,21,20]
    RIGHT_EXTENT,    TOP_EXTENT,     REAR_EXTENT, 1.0,// 20
    LEFT_EXTENT,    TOP_EXTENT,     REAR_EXTENT, 1.0,// 21
    LEFT_EXTENT,    BOTTOM_EXTENT,     REAR_EXTENT, 1.0,// 22
    RIGHT_EXTENT,    BOTTOM_EXTENT,     REAR_EXTENT,1.0, // 23

    //Object colors
    /* FRONT FACE */
    GREEN_COLOR,
    GREEN_COLOR,
    GREEN_COLOR,
    GREEN_COLOR,

    /* TOP FACE */
    BLUE_COLOR,
    BLUE_COLOR,
    BLUE_COLOR,
    BLUE_COLOR,

    /* RIGHT_FACE */
    RED_COLOR,
    RED_COLOR,
    RED_COLOR,
    RED_COLOR,

    /* BOTTOM FACE */
    GREY_COLOR,
    GREY_COLOR,
    GREY_COLOR,
    GREY_COLOR,

    /* LEFT FACE */
    BROWN_COLOR,
    BROWN_COLOR,
    BROWN_COLOR,
    BROWN_COLOR,

    /* BACK FACE */
    YELLOW_COLOR,
    YELLOW_COLOR,
    YELLOW_COLOR,
    YELLOW_COLOR
};

const GLshort indexData[] =
{
//     FRONT FACE
    0,1,2,
    2,3,0,

    // TOP FACE
    4,5,6,
    6,7,4,

    // RIGHT_FACE
    8,9,10,
    10,11,8,

    // BOTTOM FACE
    12,13,14,
    14,15,12,

    // LEFT FACE
    16,17,18,
    18,19,16,

    // BACK FACE
    20,21,22,
    22,23,20,
};

Cube::Cube() : vertexBuffer(QOpenGLBuffer::VertexBuffer), indexBuffer(QOpenGLBuffer::IndexBuffer)
{
    initBuffers();
}

void Cube::draw()
{
    mainVAO.bind(); CE()
    glDrawElements(GL_TRIANGLES, sizeof(indexData)/sizeof(short), GL_UNSIGNED_SHORT, 0); CE()
    mainVAO.release();
}

void Cube::drawSelection()
{
    selectionVAO.bind(); CE()
    glDrawElements(GL_TRIANGLES, sizeof(indexData)/sizeof(short), GL_UNSIGNED_SHORT, 0); CE()
    selectionVAO.release();
}

void Cube::initBuffers()
{
    /* Inititalize and fill the buffers */
    vertexBuffer.create(); CE()
    indexBuffer.create();CE()

    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);CE()
    vertexBuffer.bind();CE()
    vertexBuffer.allocate(vertexData, sizeof(vertexData));//24 * 4 * sizeof(float);//sizeof(positionData));CE()

    indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);CE()
    indexBuffer.bind();CE()
    indexBuffer.allocate(indexData, sizeof(indexData));//sizeof(colorData));CE()

    indexBuffer.release();
    vertexBuffer.release();
}

void Cube::initVAO(QOpenGLShaderProgram * mainShader, QOpenGLShaderProgram * selectionShader)
{
    /* MAIN VAO */
    mainShader->bind();
    /* Inititalize the VAO */
    mainVAO.create();CE()
    mainVAO.bind();CE()

    vertexBuffer.bind();CE()
    indexBuffer.bind();CE()

    mainShader->enableAttributeArray(0);
    mainShader->enableAttributeArray(1);
    mainShader->setAttributeBuffer(0, GL_FLOAT, 0, 4);CE()
    mainShader->setAttributeBuffer(1, GL_FLOAT, sizeof(float)*4*NUMBER_OF_VERTICES, 4);CE()

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(float)*4*NUMBER_OF_VERTICES));

    mainVAO.release();CE()
    vertexBuffer.release();CE()
    indexBuffer.release();CE()

    mainShader->release();

    /* SELECTION VAO */
    selectionShader->bind();
    /* Inititalize the VAO */
    selectionVAO.create();CE()
    selectionVAO.bind();CE()

    vertexBuffer.bind();CE()
    indexBuffer.bind();CE()

    selectionShader->enableAttributeArray(0);
    selectionShader->enableAttributeArray(1);
    selectionShader->setAttributeBuffer(0, GL_FLOAT, 0, 4);CE()
    selectionShader->setAttributeBuffer(1, GL_FLOAT, sizeof(float)*4*NUMBER_OF_VERTICES, 4);CE()

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*) (sizeof(float)*4*NUMBER_OF_VERTICES));

    selectionVAO.release();CE()
    vertexBuffer.release();CE()
    indexBuffer.release();CE()

    selectionShader->release();
}
