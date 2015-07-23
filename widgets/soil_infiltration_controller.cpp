#include "soil_infiltration_controller.h"

#define DEFAULT_SOIL_ABSORPTION 2
#define MIN_SOIL_ABSORPTION 0
#define MAX_SOIL_ABSORPTION 50
SoilInfiltrationSlider::SoilInfiltrationSlider(QWidget * parent): QSlider(Qt::Vertical, parent)
{
    setRange(MIN_SOIL_ABSORPTION, MAX_SOIL_ABSORPTION);
//    setTickInterval(1);
    setValue(DEFAULT_SOIL_ABSORPTION);
}

SoilInfiltrationSlider::~SoilInfiltrationSlider()
{

}

const float SoilInfiltrationControllerWidget::_MIN_INFILTRATION_RATE(MIN_SOIL_ABSORPTION);
const float SoilInfiltrationControllerWidget::_MAX_INFILTRATION_RATE(MAX_SOIL_ABSORPTION);
SoilInfiltrationControllerWidget::SoilInfiltrationControllerWidget(int alignment, QWidget * parent, Qt::WindowFlags f) :
    BaseSliderControllerWidget(new SoilInfiltrationSlider, "Soil Absorption Rate", alignment, parent, f)
{
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(refresh_label(int)));
    refresh_label(m_slider->value());
}

SoilInfiltrationControllerWidget::~SoilInfiltrationControllerWidget()
{

}

int SoilInfiltrationControllerWidget::getInfiltration() const
{
    return m_slider->value();
}

void SoilInfiltrationControllerWidget::refresh_label(int value)
{
    m_value_label->setText(QString::number(value));
}
