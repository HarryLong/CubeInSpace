#include "time_controller_dialog.h"
#include "constants.h"

#include <QBoxLayout>
#include <QLabel>\

#include <iostream>

MonthOfYearSlider::MonthOfYearSlider()
{
    setOrientation(Qt::Horizontal);
    setRange(1,12);
    setValue(DEFAULT_MONTH_OF_YEAR);
}

TimeOfDaySlider::TimeOfDaySlider()
{
    setOrientation(Qt::Horizontal);
    setRange(0,1439);
    setValue(DEFAULT_TIME_OF_DAY);
}


TimeControllerDialog::TimeControllerDialog(QWidget * parent, Qt::WindowFlags f) :
    m_month_of_year_slider(new MonthOfYearSlider), m_time_of_day_slider(new TimeOfDaySlider),
    m_month_lbl(new QLabel), m_time_lbl(new QLabel)
{
    setModal(false);
    connect(m_month_of_year_slider, SIGNAL(valueChanged(int)), this, SLOT(refresh_labels()));
    connect(m_time_of_day_slider, SIGNAL(valueChanged(int)), this, SLOT(refresh_labels()));

    init_layout();
}

TimeControllerDialog::~TimeControllerDialog()
{

}

QSize TimeControllerDialog::sizeHint() const
{
    return QSize(500, 200);
}

void TimeControllerDialog::init_layout()
{
    refresh_labels(); // Add content to labels

    QVBoxLayout * main_layout = new QVBoxLayout;

    // Month of year
    {
        QHBoxLayout * layout (new QHBoxLayout);
        // Label
        layout->addWidget(new QLabel("Month: "),0);
        // Slider
        layout->addWidget(m_month_of_year_slider,1);
        // Value label
        layout->addWidget(m_month_lbl, 0);

        main_layout->addLayout(layout,0);
    }

    // Time of day
    {
        QHBoxLayout * layout (new QHBoxLayout);
        // Title Label
        layout->addWidget(new QLabel("Time: "),0);
        // Slider
        layout->addWidget(m_time_of_day_slider,1);
        // Value label
        layout->addWidget(m_time_lbl, 0);

        main_layout->addLayout(layout,0);
    }

    // Padding
    main_layout->addWidget(new QLabel(""),1);

    setLayout(main_layout);
}

int TimeControllerDialog::getTimeOfDay()
{
    return m_time_of_day_slider->value();
}

int TimeControllerDialog::getMonthOfYear()
{
    return m_month_of_year_slider->value();
}

void TimeControllerDialog::refresh_labels()
{
    m_time_lbl->setText(format_time(getTimeOfDay()));
    m_month_lbl->setText(g_months[getMonthOfYear()-1]);
}

QString TimeControllerDialog::format_time(int minutes)
{
    int h(minutes/60);
    int mn(minutes % 60);

    QString formatted_time(get_two_number_digit(h));
    formatted_time.append("h").append(get_two_number_digit(mn));
    return formatted_time;
}

QString TimeControllerDialog::get_two_number_digit(int digit)
{
    return QString("%1").arg(digit, 2, 10, QChar('0'));
}

