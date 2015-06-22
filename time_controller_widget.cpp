#include "time_controller_widget.h"
#include <QBoxLayout>
#include <QLabel>
#include "constants.h"

MonthSlider::MonthSlider(QWidget * parent) : QSlider(Qt::Vertical, parent)
{
    setRange(1,12);
    setValue(DEFAULT_MONTH_OF_YEAR);
}

TimeSlider::TimeSlider(QWidget * parent) : QSlider(Qt::Vertical, parent)
{
    setRange(0,1439);
    setValue(DEFAULT_TIME_OF_DAY);
}

//------------------------------------
#define LATITUDE_CONTROLLER_HEIGHT 400
#define LATITUDE_CONTROLLER_WIDTH 100
TimeControllerWidget::TimeControllerWidget(QWidget * parent, Qt::WindowFlags f) : QWidget(parent, f),
    m_time_slider(new TimeSlider(this)), m_month_slider(new MonthSlider(this)), m_lbl(new QLabel(this))
{
    setFixedSize(LATITUDE_CONTROLLER_WIDTH, LATITUDE_CONTROLLER_HEIGHT);
    init_layout();

    connect(m_time_slider, SIGNAL(valueChanged(int)), this, SLOT(refresh_label()));
    connect(m_month_slider, SIGNAL(valueChanged(int)), this, SLOT(refresh_label()));
}

TimeControllerWidget::~TimeControllerWidget()
{

}

int TimeControllerWidget::getTimeOfDay() const
{
    return m_time_slider->value();
}

int TimeControllerWidget::getMonthOfYear() const
{
    return m_month_slider->value();
}

void TimeControllerWidget::init_layout()
{
    QVBoxLayout * main_layout(new QVBoxLayout);

    // Sliders
    {
        QHBoxLayout * slider_layout(new QHBoxLayout);
        slider_layout->addWidget(m_month_slider, 0);
        slider_layout->addWidget(m_time_slider, 0);

        main_layout->addLayout(slider_layout,1);
    }

    // Label
    main_layout->addWidget(m_lbl,0);

    setLayout(main_layout);
}

void TimeControllerWidget::refresh_label()
{
    m_lbl->setText("THIS IS A TEST");
}
