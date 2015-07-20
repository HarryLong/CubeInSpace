#include "latitude_controller.h"

/******************************
 * LATITUDE CONTROLLER WIDGET *
 ******************************/
#define DEFAULT_LATITUDE 0
LatitudeSlider::LatitudeSlider(QWidget * parent) : QSlider(Qt::Vertical, parent)
{
    setRange(-90,90);
    setTickInterval(10);
    setValue(DEFAULT_LATITUDE);
}

LatitudeSlider::~LatitudeSlider()
{

}

LatitudeControllerWidget::LatitudeControllerWidget(int alignment, QWidget * parent, Qt::WindowFlags f) :
    BaseSliderControllerWidget(new LatitudeSlider, "LATITUDE", alignment, parent, f)
{
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(refresh_label(int)));
    refresh_label(m_slider->value());
}

LatitudeControllerWidget::~LatitudeControllerWidget()
{

}

int LatitudeControllerWidget::getLatitude() const
{
    return value();
}

void LatitudeControllerWidget::refresh_label(int value)
{
    m_value_label->setText(QString::number(value));
}
