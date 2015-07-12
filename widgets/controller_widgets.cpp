#include "controller_widgets.h"
#include <QBoxLayout>
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
OverlayWidgets::OverlayWidgets(QWidget * parent) :
    m_time_widget(new TimeControllerWidget(Qt::AlignRight, parent)),
    m_month_widget(new MonthControllerWidget(Qt::AlignLeft, parent)),
    m_latitude_widget(new LatitudeControllerWidget(Qt::AlignRight, parent))
{
    m_alignment_sorted_widgets[Qt::AlignTop] = std::vector<BaseControllerWidget*>();
    m_alignment_sorted_widgets[Qt::AlignRight] = std::vector<BaseControllerWidget*>();
    m_alignment_sorted_widgets[Qt::AlignBottom] = std::vector<BaseControllerWidget*>();
    m_alignment_sorted_widgets[Qt::AlignLeft] = std::vector<BaseControllerWidget*>();

    // LATITUDE
    {
        connect(m_latitude_widget->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(emit_latitude_changed(int)));

        m_raw_widgets.push_back(m_latitude_widget);
        m_alignment_sorted_widgets[m_latitude_widget->alignment()].push_back(m_latitude_widget);
    }

    // TIME
    {
        connect(m_month_widget->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(emit_month_changed(int)));
        connect(m_time_widget->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(emit_time_changed(int)));

        m_raw_widgets.push_back(m_month_widget);
        m_raw_widgets.push_back(m_time_widget);

        m_alignment_sorted_widgets[m_month_widget->alignment()].push_back(m_month_widget);
        m_alignment_sorted_widgets[m_time_widget->alignment()].push_back(m_time_widget);
    }
    hideAll();
}

OverlayWidgets::~OverlayWidgets()
{
    for(BaseControllerWidget* w : m_raw_widgets)
        delete w;
}

int OverlayWidgets::getLatitude()
{
    return m_latitude_widget->value();
}

int OverlayWidgets::getTime()
{
    return m_time_widget->value();
}

int OverlayWidgets::getMonth()
{
    return m_month_widget->value();
}

void OverlayWidgets::hideAll()
{
    for(BaseControllerWidget* w : m_raw_widgets)
        w->hide();
}

void OverlayWidgets::trigger_time_controllers(bool show)
{
    hideAll();
    if(show)
    {
        m_time_widget->show();
        m_month_widget->show();
    }
    else
    {
        m_time_widget->hide();
        m_month_widget->hide();
    }

    emit timeControllersStateChanged(show);
}

void OverlayWidgets::trigger_latitude_controllers(bool show)
{
    hideAll();
    if(show)
    {
        m_latitude_widget->show();
    }
    else
    {
        m_latitude_widget->hide();
    }

    emit latitudeControllersStateChanged(show);
}


void OverlayWidgets::resize(int container_width, int container_height)
{
    for(BaseControllerWidget * w : m_alignment_sorted_widgets[Qt::AlignRight])
        w->move(container_width-w->width(), 0);

    for(BaseControllerWidget * w : m_raw_widgets)
    {
        w->setFixedHeight(container_height);
        w->resize();
    }
}

void OverlayWidgets::emit_latitude_changed(int latitude)
{
    emit latitudeChanged(latitude);
}

void OverlayWidgets::emit_time_changed(int time)
{
    emit timeChanged(time);
}

void OverlayWidgets::emit_month_changed(int month)
{
    emit monthChanged(month);
}
