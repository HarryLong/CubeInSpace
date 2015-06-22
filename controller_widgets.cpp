#include "controller_widgets.h"
#include <QBoxLayout>
#include <QLabel>
#include "constants.h"

/**************************
 * BASE CONTROLLER WIDGET *
 **************************/
BaseControllerWidget::BaseControllerWidget(QSlider * slider, QWidget * parent, Qt::WindowFlags f) :
    QWidget(parent), m_slider(slider), m_label(new QLabel)
{
    init_layout();
}

BaseControllerWidget::~BaseControllerWidget()
{

}

QSlider * BaseControllerWidget::getSlider()
{
    return m_slider;
}

int BaseControllerWidget::value() const
{
    return m_slider->value();
}

void BaseControllerWidget::init_layout()
{
    QVBoxLayout * main_layout(new QVBoxLayout);

    main_layout->addWidget(m_slider, 1, Qt::AlignHCenter);
    main_layout->addWidget(m_label,0, Qt::AlignHCenter);

    setLayout(main_layout);
}

/******************************
 * LATITUDE CONTROLLER WIDGET *
 ******************************/
#define DEFAULT_LATITUDE 0
LatitudeSlider::LatitudeSlider(QWidget * parent) : QSlider(Qt::Vertical, parent)
{
    setRange(-90,90);
    setValue(DEFAULT_LATITUDE);
}

LatitudeSlider::~LatitudeSlider()
{

}

LatitudeControllerWidget::LatitudeControllerWidget(QWidget * parent, Qt::WindowFlags f) : BaseControllerWidget(new LatitudeSlider, parent, f)
{
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(refresh_label(int)));
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
    m_label->setText(QString::number(value));
}

/**************************
 * TIME CONTROLLER WIDGET *
 **************************/
TimeSlider::TimeSlider(QWidget * parent) : QSlider(Qt::Vertical, parent)
{
    setRange(0,1439);
    setValue(DEFAULT_TIME_OF_DAY);
}

TimeSlider::~TimeSlider()
{

}

TimeControllerWidget::TimeControllerWidget(QWidget * parent, Qt::WindowFlags f) : BaseControllerWidget(new TimeSlider, parent, f)
{
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(refresh_label(int)));
}

TimeControllerWidget::~TimeControllerWidget()
{

}

int TimeControllerWidget::getTime() const
{
    return value();
}

TimeControllerWidget::refresh_label(int value)
{
    m_label->setText(QString::number(value));
}

/***************************
 * MONTH CONTROLLER WIDGET *
 ***************************/
MonthSlider::MonthSlider(QWidget * parent) : QSlider(Qt::Vertical, parent)
{
    setRange(1,12);
    setValue(DEFAULT_MONTH_OF_YEAR);
}

MonthSlider::~MonthSlider()
{

}

MonthControllerWidget::MonthControllerWidget(QWidget * parent, Qt::WindowFlags f) : BaseControllerWidget(new MonthSlider, parent, f)
{
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(refresh_label(int)));
}

MonthControllerWidget::~MonthControllerWidget()
{

}

int MonthControllerWidget::getMonth() const
{

}

void MonthControllerWidget::refresh_label(int value)
{
    m_label->setText(QString::number(value));
}
