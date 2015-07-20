#include "time_controller.h"

/**************************
 * TIME CONTROLLER WIDGET *
 **************************/
#define DEFAULT_TIME_OF_DAY 720 // MIDDAY IN MINUTES
TimeSlider::TimeSlider(QWidget * parent) : QSlider(Qt::Vertical, parent)
{
    setRange(0,1439);
    setTickInterval(60);
    setValue(DEFAULT_TIME_OF_DAY);
}

TimeSlider::~TimeSlider()
{

}

TimeControllerWidget::TimeControllerWidget(int alignment, QWidget * parent, Qt::WindowFlags f) :
    BaseSliderControllerWidget(new TimeSlider, "TIME OF DAY", alignment, parent, f)
{
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(refresh_label(int)));
    refresh_label(m_slider->value());
}

TimeControllerWidget::~TimeControllerWidget()
{

}

int TimeControllerWidget::getTime() const
{
    return value();
}

void TimeControllerWidget::refresh_label(int value)
{
    m_value_label->setText(format_time(value));
}

QString TimeControllerWidget::format_time(int time_in_minutes)
{
    int h(time_in_minutes/60);
    int mn(time_in_minutes%60);

    QString formatted_time(BaseSliderControllerWidget::format_two_digit_num(h));
    formatted_time.append(":").append(BaseSliderControllerWidget::format_two_digit_num(mn));

    return formatted_time;
}
