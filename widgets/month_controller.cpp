#include "month_controller.h"

/***************************
 * MONTH CONTROLLER WIDGET *
 ***************************/
#define DEFAULT_MONTH 6
MonthSlider::MonthSlider(QWidget * parent) : QSlider(Qt::Vertical, parent)
{
    setRange(1,12);
    setTickInterval(1);
    setValue(6);
}

MonthSlider::~MonthSlider()
{

}
const std::map<int,QString> MonthControllerWidget::m_num_to_month (MonthControllerWidget::create_map());
MonthControllerWidget::MonthControllerWidget(int alignment, QWidget * parent, Qt::WindowFlags f) :
    BaseSliderControllerWidget(new MonthSlider, "MONTH OF YEAR", alignment, parent, f)
{
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(refresh_label(int)));
    refresh_label(m_slider->value());
}

MonthControllerWidget::~MonthControllerWidget()
{

}

int MonthControllerWidget::getMonth() const
{
    return value();
}

void MonthControllerWidget::refresh_label(int value)
{
    m_value_label->setText(m_num_to_month.find(value)->second);
}

std::map<int, QString> MonthControllerWidget::create_map()
{
    std::map<int, QString> ret;
    ret[1] = "Jan";
    ret[2] = "Feb";
    ret[3] = "Mar";
    ret[4] = "Apr";
    ret[5] = "May";
    ret[6] = "Jun";
    ret[7] = "Jul";
    ret[8] = "Aug";
    ret[9] = "Sep";
    ret[10] = "Oct";
    ret[11] = "Nov";
    ret[12] = "Dec";

    return ret;
}
