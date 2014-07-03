#ifndef GLCONTROLWIDGET_H_
#define GLCONTROLWIDGET_H_

#include <QWidget>
#include <QScopedPointer>
#include <QComboBox>
#include <QSlider>
#include <QLabel>
#include <map>

#include "floatingpointslider.h"
#include "glwidget.h"

class QHBoxLayout;

struct ControllerData {
    float scale, xTranslation, yTranslation, zTranslation;
    int xRotation, yRotation, zRotation;

    ControllerData() : scale(1.f), xTranslation(0.f), yTranslation(0.f), zTranslation(0.f), xRotation(0), yRotation(0), zRotation(0) {}
    ControllerData(float scale, float xTranslation, float yTranslation, float zTranslation, int xRotation, int yRotation, int zRotation) :
        scale(scale), xTranslation(xTranslation), yTranslation(yTranslation), zTranslation(zTranslation), xRotation(xRotation),
        yRotation(yRotation), zRotation(zRotation) {}

};

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    ControlWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);

    // DEBUG methods
    void printControllers();

public slots:
    void slot_scaleChanged(float scale);
    void slot_xTranslationChanged(float translation);
    void slot_yTranslationChanged(float translation);
    void slot_zTranslationChanged(float translation);
    void slot_xRotationChanged(int rotation);
    void slot_yRotationChanged(int rotation);
    void slot_zRotationChanged(int rotation);
    void selectController(int code);
    void newSceneObj(int objId, float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation);

signals:
    void signal_transformationChanged(float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation);

protected:
    QComboBox* createComboBox();
    QSlider* createRotationSlider();
    FloatingPointSlider* createTranslationSlider();
    FloatingPointSlider* createScaleSlider();
    QLabel* createLabel();
    QHBoxLayout* createHorizontalLayout(QSlider* slider, QLabel* label);

    void updateLabels();
    void emitSignal();

private:
    // No need to delete these pointer as ownership will be passed to their containing layouts
    FloatingPointSlider* scaleSlider;
    FloatingPointSlider* translateXSlider;
    FloatingPointSlider* translateYSlider;
    FloatingPointSlider* translateZSlider;
    QSlider* rotateXSlider;
    QSlider* rotateYSlider;
    QSlider* rotateZSlider;

    QScopedPointer<QLabel> scaleLabel;
    QScopedPointer<QLabel> translateXLabel;
    QScopedPointer<QLabel> translateYLabel;
    QScopedPointer<QLabel> translateZLabel;
    QScopedPointer<QLabel> rotateXLabel;
    QScopedPointer<QLabel> rotateYLabel;
    QScopedPointer<QLabel> rotateZLabel;

    std::map<int, ControllerData> controllers;
    int currentControllerId;
};

#endif
