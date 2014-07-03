#include "floatingpointslider.h"

FloatingPointSlider::FloatingPointSlider(Qt::Orientation orientation, QWidget * parent) : QSlider(orientation,parent)
{
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(signalValueChanged(int)));
}

void FloatingPointSlider::setRange(float min, float max)
{
    QSlider::setRange(parseToInt(min), parseToInt(max));
}

void FloatingPointSlider::setSingleStep(float step)
{
    QSlider::setSingleStep(parseToInt(step));
}

void FloatingPointSlider::setPageStep(float step)
{
    QSlider::setPageStep(parseToInt(step));
}

void FloatingPointSlider::signalValueChanged(int value)
{
    emit floatingpointValueChanged(parseToFloat(value));
}

void FloatingPointSlider::setValue(float value)
{
    QSlider::setValue(parseToInt(value));
}

void FloatingPointSlider::setTickInterval(float value)
{
    QSlider::setTickInterval(parseToInt(value));
}

float FloatingPointSlider::floatingPointValue()
{
    return parseToFloat(QSlider::value());
}


int FloatingPointSlider::parseToInt(float value)
{
    int intValue = (int) (value * MULTIPLIER);
    return (int) (value * MULTIPLIER);
}

float FloatingPointSlider::parseToFloat(int value)
{
    float floatValue = ((float) value) / MULTIPLIER;
    return ((float) value) / MULTIPLIER;
}
