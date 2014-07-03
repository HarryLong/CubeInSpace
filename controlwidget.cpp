#include "controlwidget.h"
#include "qtutils.h"
#include "utils/utils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <string>

#define MAIN_CONTROLLER 0

ControlWidget::ControlWidget(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f), controllers(), currentControllerId(0),
    /*LABELS*/
    scaleLabel(createLabel()), translateXLabel(createLabel()), translateYLabel(createLabel()),
    translateZLabel(createLabel()), rotateXLabel(createLabel()), rotateYLabel(createLabel()),
    rotateZLabel(createLabel())
{
    controllers.insert(std::pair<int,ControllerData>(MAIN_CONTROLLER, ControllerData())); // The main controller;

    scaleSlider = createScaleSlider();
    translateXSlider = createTranslationSlider();
    translateYSlider = createTranslationSlider();
    translateZSlider = createTranslationSlider();
    rotateXSlider = createRotationSlider();
    rotateYSlider = createRotationSlider();
    rotateZSlider = createRotationSlider();

    // Initialize the labels
    updateLabels();

    // SCALE
    connect(scaleSlider, SIGNAL(floatingpointValueChanged(float)), this, SLOT(slot_scaleChanged(float)));

    // TRANSLATION
    connect(translateXSlider, SIGNAL(floatingpointValueChanged(float)), this, SLOT(slot_xTranslationChanged(float)));
    connect(translateYSlider, SIGNAL(floatingpointValueChanged(float)), this, SLOT(slot_yTranslationChanged(float)));
    connect(translateZSlider, SIGNAL(floatingpointValueChanged(float)), this, SLOT(slot_zTranslationChanged(float)));

    // ROTATION
    connect(rotateXSlider, SIGNAL(valueChanged(int)), this, SLOT(slot_xRotationChanged(int)));
    connect(rotateYSlider, SIGNAL(valueChanged(int)), this, SLOT(slot_yRotationChanged(int)));
    connect(rotateZSlider, SIGNAL(valueChanged(int)), this, SLOT(slot_zRotationChanged(int)));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    // Scale
    mainLayout->addLayout(createHorizontalLayout(scaleSlider, scaleLabel.data()));
    mainLayout->addLayout(createHorizontalLayout(translateXSlider, translateXLabel.data()));
    mainLayout->addLayout(createHorizontalLayout(translateYSlider, translateYLabel.data()));
    mainLayout->addLayout(createHorizontalLayout(translateZSlider, translateZLabel.data()));
    mainLayout->addLayout(createHorizontalLayout(rotateXSlider, rotateXLabel.data()));
    mainLayout->addLayout(createHorizontalLayout(rotateYSlider, rotateYLabel.data()));
    mainLayout->addLayout(createHorizontalLayout(rotateZSlider, rotateZLabel.data()));

    setLayout(mainLayout);
}

QComboBox* ControlWidget::createComboBox()
{
    QComboBox* cb = new QComboBox();

    cb->addItem("TEST");

    return cb;
}

QSlider* ControlWidget::createRotationSlider()
{
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(-360, 360);
    slider->setSingleStep(1);
    slider->setPageStep(10);
    slider->setTickInterval(20);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

FloatingPointSlider* ControlWidget::createTranslationSlider()
{
    FloatingPointSlider *slider = new FloatingPointSlider(Qt::Horizontal);
    slider->setRange(-5.f,5.f);
    slider->setSingleStep(.1f);
    slider->setPageStep(.5f);
    slider->setTickInterval(1.f);
    slider->setTickPosition(QSlider::TicksRight);
    slider->setValue(0.f);
    return slider;
}

FloatingPointSlider* ControlWidget::createScaleSlider()
{
    FloatingPointSlider *slider = new FloatingPointSlider(Qt::Horizontal);
    slider->setRange(0.1f, 10.0f);
    slider->setSingleStep(.1f);
    slider->setPageStep(.2f);
    slider->setTickInterval(1.f);
    slider->setTickPosition(QSlider::TicksRight);
    slider->setValue(1.f);
    return slider;
}

QLabel* ControlWidget::createLabel()
{
    static QFont boldFont = QTUTILS::getFont(QTUTILS::bold);

    QLabel * scaleLabel = new QLabel(this);
    scaleLabel->setFont(boldFont);
    return scaleLabel;
}

QHBoxLayout* ControlWidget::createHorizontalLayout(QSlider* slider, QLabel* label)
{
    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(slider,6);
    layout->addWidget(label,1, Qt::AlignRight);

    return layout;
}

void ControlWidget::updateLabels()
{
    // Scale
    std::string lbl = std::string("Scale [") + std::to_string(scaleSlider->floatingPointValue()) + "X]";
    scaleLabel.data()->setText(lbl.c_str());

    // X Translation
    lbl = std::string("X Translation [") + std::to_string(translateXSlider->floatingPointValue()) + "]";
    translateXLabel.data()->setText(lbl.c_str());

    // Y Translation
    lbl = std::string("Y Translation [") + std::to_string(translateYSlider->floatingPointValue()) + "]";
    translateYLabel.data()->setText(lbl.c_str());

    // Z Translation
    lbl = std::string("Z Translation [") + std::to_string(translateZSlider->floatingPointValue()) + "]";
    translateZLabel.data()->setText(lbl.c_str());

    // X Rotation
    lbl = std::string("X Rotation [") + std::to_string(rotateXSlider->value()) + "]";
    rotateXLabel.data()->setText(lbl.c_str());

    // Y Rotation
    lbl = std::string("Y Rotation [") + std::to_string(rotateYSlider->value()) + "]";
    rotateYLabel.data()->setText(lbl.c_str());

    // Z Rotation
    lbl = std::string("Z Rotation [") + std::to_string(rotateZSlider->value()) + "]";
    rotateZLabel.data()->setText(lbl.c_str());
}

void ControlWidget::slot_scaleChanged(float scale)
{
    controllers[currentControllerId].scale = scale;
    updateLabels();
    emitSignal();
}

void ControlWidget::slot_xTranslationChanged(float translation)
{
    controllers[currentControllerId].xTranslation = translation;
    updateLabels();
    emitSignal();
}

void ControlWidget::slot_yTranslationChanged(float translation)
{
    controllers[currentControllerId].yTranslation = translation;
    updateLabels();
    emitSignal();
}

void ControlWidget::slot_zTranslationChanged(float translation)
{
    controllers[currentControllerId].zTranslation = translation;
    updateLabels();
    emitSignal();
}

void ControlWidget::slot_xRotationChanged(int rotation)
{
    controllers[currentControllerId].xRotation = rotation;
    updateLabels();
    emitSignal();
}

void ControlWidget::slot_yRotationChanged(int rotation)
{
    controllers[currentControllerId].yRotation = rotation;
    updateLabels();
    emitSignal();
}

void ControlWidget::slot_zRotationChanged(int rotation)
{
    controllers[currentControllerId].zRotation = rotation;
    updateLabels();
    emitSignal();
}

void ControlWidget::emitSignal()
{
    emit signal_transformationChanged(scaleSlider->floatingPointValue(), translateXSlider->floatingPointValue(), translateYSlider->floatingPointValue(),
                                      translateZSlider->floatingPointValue(), rotateXSlider->value(), rotateYSlider->value(), rotateZSlider->value());
}

void ControlWidget::newSceneObj(int objId, float scale, float xTranslation, float yTranslation,float zTranslation, int xRotation, int yRotation, int zRotation)
{
    ControllerData controller(scale, xTranslation, yTranslation, zTranslation, xRotation, yRotation, zRotation);
    controllers.insert(std::pair<int,ControllerData>(objId, controller));

    printControllers();
}

void ControlWidget::selectController(int code)
{
    if(code != currentControllerId)
    {
        std::map<int, ControllerData>::iterator it;
        if((it = controllers.find(code)) != controllers.end())
        {
            currentControllerId = code;

            scaleSlider->setValue(it->second.scale);
            translateXSlider->setValue(it->second.xTranslation);
            translateYSlider->setValue(it->second.yTranslation);
            translateZSlider->setValue(it->second.zTranslation);
            rotateXSlider->setValue(it->second.xRotation);
            rotateYSlider->setValue(it->second.yRotation);
            rotateZSlider->setValue(it->second.zRotation);

            updateLabels();

        }
    }
}

/* DEBUG METHOD */
void ControlWidget::printControllers()
{
    std::cout << "*********CONTROLLERS*********" << std::endl;
    for(std::map<int, ControllerData>::iterator it = controllers.begin(); it != controllers.end(); ++it)
    {
        std::cout << "-- Controller " << it->first << std::endl;
        std::cout << "Scale: " << it->second.scale << std::endl;
        std::cout << "X translation: " << it->second.xTranslation << std::endl;
        std::cout << "Y translation: " << it->second.yTranslation << std::endl;
        std::cout << "Z translation: " << it->second.zTranslation << std::endl;
        std::cout << "X rotation: " << it->second.xRotation << std::endl;
        std::cout << "Y rotation: " << it->second.yRotation << std::endl;
        std::cout << "Z rotation: " << it->second.zRotation << std::endl;
        std::cout << "--" << std::endl << std::endl;
    }
    std::cout << "*****************************" << std::endl;
}
