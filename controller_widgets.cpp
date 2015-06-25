#include "controller_widgets.h"
#include <QBoxLayout>
#include "constants.h"
#include <QDebug>
#include <QPainter>
#include <QAction>

DisplayLabel::DisplayLabel(QWidget *parent) :  QLabel(parent)
{
    setStyleSheet("QLabel { color : rgb(120,120,120); }");
    setAlignment(Qt::AlignBottom);
}

HeaderLabel::HeaderLabel(QString text, QWidget * parent) :
    DisplayLabel(parent)
{
    setText(text);
}

void HeaderLabel::paintEvent(QPaintEvent*)
{
    setAlignment(Qt::AlignCenter);
    QPainter painter(this);
    painter.rotate(90);
    painter.drawText(QPoint(0,0), text());
}

/**************************
 * BASE CONTROLLER WIDGET *
 **************************/
const QFont BaseControllerWidget::m_label_font("Times", 25, QFont::Weight::Bold);
BaseControllerWidget::BaseControllerWidget(QSlider * slider, QString heading, int alignment, QWidget * parent, Qt::WindowFlags f) :
    QWidget(parent, f), m_slider(slider), m_value_label(new DisplayLabel(this)), m_heading_label(new HeaderLabel(heading,  this)),
    m_alignment(alignment)
{
    if(alignment == Qt::AlignRight)
        m_slider->setTickPosition(QSlider::TickPosition::TicksLeft);
    else if(alignment == Qt::AlignLeft)
        m_slider->setTickPosition(QSlider::TickPosition::TicksRight);

    init_layout();
    setFixedWidth(100);
    m_value_label->setFont(m_label_font);
    m_heading_label->setFont(m_label_font);

    QFontMetrics fm(m_label_font);
    m_value_label->setFixedHeight(fm.height()+15);
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

    // Slider and header
    {
        QHBoxLayout * layout(new QHBoxLayout);
        if(m_alignment == Qt::AlignLeft)
        {
            layout->addWidget(m_slider, 0, Qt::AlignLeft);
            layout->addWidget(m_heading_label, 0, Qt::AlignRight);
        }
        else
        {
            layout->addWidget(m_heading_label, 0, Qt::AlignLeft);
            layout->addWidget(m_slider, 0, Qt::AlignRight);
        }

        main_layout->addLayout(layout, 0);
    }
    main_layout->addWidget(m_value_label,0, Qt::AlignHCenter | Qt::AlignBottom);

    setLayout(main_layout);
}

void BaseControllerWidget::resize()
{
    int h(height());
    m_slider->setFixedHeight(h-m_value_label->height());
}

int BaseControllerWidget::alignment()
{
    return m_alignment;
}

QString BaseControllerWidget::format_two_digit_num(int digit)
{
    return QString("%1").arg(digit, 2, 10, QChar('0'));
}

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
    BaseControllerWidget(new LatitudeSlider, "LATITUDE", alignment, parent, f)
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

/**************************
 * TIME CONTROLLER WIDGET *
 **************************/
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
    BaseControllerWidget(new TimeSlider, "TIME OF DAY", alignment, parent, f)
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

    QString formatted_time(BaseControllerWidget::format_two_digit_num(h));
    formatted_time.append(":").append(BaseControllerWidget::format_two_digit_num(mn));

    return formatted_time;
}

/***************************
 * MONTH CONTROLLER WIDGET *
 ***************************/
MonthSlider::MonthSlider(QWidget * parent) : QSlider(Qt::Vertical, parent)
{
    setRange(1,12);
    setTickInterval(1);
    setValue(DEFAULT_MONTH_OF_YEAR);
}

MonthSlider::~MonthSlider()
{

}
const std::map<int,QString> MonthControllerWidget::m_num_to_month (MonthControllerWidget::create_map());
MonthControllerWidget::MonthControllerWidget(int alignment, QWidget * parent, Qt::WindowFlags f) :
    BaseControllerWidget(new MonthSlider, "MONTH OF YEAR", alignment, parent, f)
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

/******************************
 * CONTROLLER WIDGETS WRAPPER *
 ******************************/
ControllerWidgetsWrapper::ControllerWidgetsWrapper(QAction * latitude_trigger_action, QAction * time_of_day_trigger_action,
                               QAction * month_of_year_trigger_action, QWidget * parent)
{
    m_alignment_sorted_widgets[Qt::AlignTop] = std::vector<ActionAndWidgetPair>();
    m_alignment_sorted_widgets[Qt::AlignRight] = std::vector<ActionAndWidgetPair>();
    m_alignment_sorted_widgets[Qt::AlignBottom] = std::vector<ActionAndWidgetPair>();
    m_alignment_sorted_widgets[Qt::AlignLeft] = std::vector<ActionAndWidgetPair>();

    // LATITUDE
    {
        ControllerWidgetsWrapper::ActionAndWidgetPair awp(latitude_trigger_action, new LatitudeControllerWidget(Qt::AlignRight, parent));
        m_widgets.insert(std::pair<Type, ActionAndWidgetPair>(Type::_LATITUDE, awp));
        m_alignment_sorted_widgets[awp.widget->alignment()].push_back(awp);
    }

    // MONTH OF YEAR
    {
        ControllerWidgetsWrapper::ActionAndWidgetPair awp(month_of_year_trigger_action, new MonthControllerWidget(Qt::AlignRight, parent));
        m_widgets.insert(std::pair<Type, ActionAndWidgetPair>(Type::_MONTH, awp));
        m_alignment_sorted_widgets[awp.widget->alignment()].push_back(awp);
    }

    // TIME OF DAY
    {
        ControllerWidgetsWrapper::ActionAndWidgetPair awp(time_of_day_trigger_action, new TimeControllerWidget(Qt::AlignLeft, parent));
        m_widgets.insert(std::pair<Type, ActionAndWidgetPair>(Type::_TIME, awp));
        m_alignment_sorted_widgets[awp.widget->alignment()].push_back(awp);
    }

    // Trigger overlay widgets
    connect(latitude_trigger_action, SIGNAL(triggered(bool)), this, SLOT(trigger_latitude_overlay(bool)));
    connect(time_of_day_trigger_action, SIGNAL(triggered(bool)), this, SLOT(trigger_time_overlay(bool)));
    connect(month_of_year_trigger_action, SIGNAL(triggered(bool)), this, SLOT(trigger_month_overlay(bool)));

    hideAll();
}

ControllerWidgetsWrapper::~ControllerWidgetsWrapper()
{
    for(std::pair<Type, ActionAndWidgetPair> w : m_widgets)
        delete w.second.widget;
}

void ControllerWidgetsWrapper::show(Type type)
{
    ActionAndWidgetPair w_pair = m_widgets.find(type)->second;
    for(ActionAndWidgetPair awp : m_alignment_sorted_widgets[w_pair.widget->alignment()])
    {
        awp.widget->hide();
        awp.action->setChecked(false);
    }
    w_pair.widget->show();
    w_pair.action->setChecked(true);
}

void ControllerWidgetsWrapper::hide(Type type)
{
    ActionAndWidgetPair w_pair = m_widgets.find(type)->second;
    w_pair.widget->hide();
    w_pair.action->setChecked(false);
}

void ControllerWidgetsWrapper::hideAll()
{
    for(int type(0); type < Type::_OVERLAY_WIDGET_COUNT; type++)
        hide(static_cast<Type>(type));
}

bool ControllerWidgetsWrapper::isVisible(Type type) const
{
    return m_widgets.find(type)->second.widget->isVisible();
}

BaseControllerWidget * ControllerWidgetsWrapper::operator()(Type type)
{
    return m_widgets.find(type)->second.widget;
}

void ControllerWidgetsWrapper::trigger_time_overlay(bool checked)
{
    if(checked)
        show(Type::_TIME);
    else
        hide(Type::_TIME);
}

void ControllerWidgetsWrapper::trigger_month_overlay(bool checked)
{
    if(checked)
        show(Type::_MONTH);
    else
        hide(Type::_MONTH);
}

void ControllerWidgetsWrapper::trigger_latitude_overlay(bool checked)
{
    if(checked)
        show(Type::_LATITUDE);
    else
        hide(Type::_LATITUDE);
}

void ControllerWidgetsWrapper::resize(int container_width, int container_height)
{
    for(ActionAndWidgetPair awp : m_alignment_sorted_widgets[Qt::AlignRight])
        awp.widget->move(container_width-awp.widget->width(), 0);

    for(std::pair<Type,ActionAndWidgetPair> awp : m_widgets)
    {
        awp.second.widget->setFixedHeight(container_height);
        awp.second.widget->resize();
    }
}
