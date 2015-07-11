#include "rainfall_edit_dlg.h"
#include <QLineEdit>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>

/********************************
 * RAINFALL INTENSITY LINE EDIT *
 ********************************/
#define MIN_RAINFALL_INTENSITY 0
#define MAX_RAINFALL_INTENSITY 1000
#define DEFAULT_RAINFALL_INTENSITY 0
RainfallIntensityLineEdit::RainfallIntensityLineEdit(QWidget * parent) :
    QIntLineEdit(MIN_RAINFALL_INTENSITY, MAX_RAINFALL_INTENSITY, DEFAULT_RAINFALL_INTENSITY, parent)
{

}

RainfallIntensityLineEdit::~RainfallIntensityLineEdit()
{

}

/**********************
 * RAINFALL LINE EDIT *
 **********************/
#define MIN_RAINFALL 0
#define MAX_RAINFALL 50000
#define DEFAULT_RAINFALL 0
MonthlyRainfallLineEdit::MonthlyRainfallLineEdit(QWidget * parent) :
    QIntLineEdit(MIN_RAINFALL, MAX_RAINFALL, DEFAULT_RAINFALL, parent)
{

}

MonthlyRainfallLineEdit::~MonthlyRainfallLineEdit()
{

}

/*****************************
 * TEMPERATURE EDITOR WIDGET *
 *****************************/
RainfallEditWidget::RainfallEditWidget(QWidget * parent, Qt::WindowFlags f) :
    QWidget(parent, f), m_monthly_rainfall_le(new MonthlyRainfallLineEdit(this)), m_rainfall_intensity_le(new RainfallIntensityLineEdit(this))
{
    init_layout();
}

RainfallEditWidget::~RainfallEditWidget()
{

}

void RainfallEditWidget::init_layout()
{
    QVBoxLayout * main_layout (new QVBoxLayout);

    // Monthly rainfall
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Monthly rainfall (mm): "),2, Qt::AlignLeft);
        layout->addWidget(m_monthly_rainfall_le, 1, Qt::AlignRight);

        main_layout->addLayout(layout);
    }

    // Rainfal intensity
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Rainfall intensity (mm/h): "),2, Qt::AlignLeft);
        layout->addWidget(m_rainfall_intensity_le, 1, Qt::AlignRight);

        main_layout->addLayout(layout);
    }

    setLayout(main_layout);
}

int RainfallEditWidget::getMonthlyRainfall()
{
    return m_monthly_rainfall_le->value();
}

int RainfallEditWidget::getRainfallIntensity()
{
    return m_rainfall_intensity_le->value();
}

void RainfallEditWidget::reset()
{
    m_monthly_rainfall_le->reset();
    m_rainfall_intensity_le->reset();
}

void RainfallEditWidget::pop()
{
    m_monthly_rainfall_le->setText(m_cached_monthly_rainfall);
    m_rainfall_intensity_le->setText(m_cached_rainfall_intensity);
}

void RainfallEditWidget::push()
{
    m_cached_monthly_rainfall = m_monthly_rainfall_le->text();
    m_cached_rainfall_intensity = m_rainfall_intensity_le->text();
}

/*****************************
 * TEMPERATURE EDITOR DIALOG *
 *****************************/
RainfallEditDialog::RainfallEditDialog(QWidget * parent, Qt::WindowFlags f ) :
    QDialog(parent,f), m_jun_rainfall_widget(new RainfallEditWidget), m_dec_rainfall_widget(new RainfallEditWidget),
    m_ok_btn(new QPushButton("OK")), m_cancel_btn(new QPushButton("Cancel")), m_info_lbl(new QLabel(this))
{
    setWindowTitle("Rainfall Editor");

    init_layout();

    connect(m_ok_btn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_cancel_btn, SIGNAL(clicked()), this, SLOT(reject()));
}

RainfallEditDialog::~RainfallEditDialog()
{

}

RainfallEditDialog::RainfallAttributes RainfallEditDialog::getJunRainfallAttributes()
{
    return RainfallAttributes(m_jun_rainfall_widget->getMonthlyRainfall(), m_jun_rainfall_widget->getRainfallIntensity());
}

RainfallEditDialog::RainfallAttributes RainfallEditDialog::getDecRainfallAttributes()
{
    return RainfallAttributes(m_dec_rainfall_widget->getMonthlyRainfall(), m_dec_rainfall_widget->getRainfallIntensity());
}

void RainfallEditDialog::init_layout()
{
    QVBoxLayout * main_layout (new QVBoxLayout);

    // Minimum temperature
    {
        main_layout->addWidget(new QLabel("<html> <span style=\"font-size:14pt; font-weight:600;\"> JUNE </span> </html>"),0, Qt::AlignCenter);
        main_layout->addWidget(m_jun_rainfall_widget,0);
    }

    // Maximum temperature
    {
        main_layout->addWidget(new QLabel("<html> <span style=\"font-size:14pt; font-weight:600;\"> DECEMBER </span> </html>"),0, Qt::AlignCenter);
        main_layout->addWidget(m_dec_rainfall_widget,0);
    }

    main_layout->addWidget(m_info_lbl,0, Qt::AlignCenter);

    main_layout->addWidget(new QLabel(""),1); // Padding

    // OK / Cancel button
    {
        QHBoxLayout * layout (new QHBoxLayout);
        layout->addWidget(m_cancel_btn, 1, Qt::AlignRight);
        layout->addWidget(m_ok_btn, 1, Qt::AlignLeft);

        main_layout->addLayout(layout, 0);
    }

    setLayout(main_layout);
}

void RainfallEditDialog::reject()
{
    m_jun_rainfall_widget->pop();
    m_dec_rainfall_widget->pop();

    QDialog::reject();
}

void RainfallEditDialog::accept()
{
    emit rainfallValuesChanged(m_jun_rainfall_widget->getMonthlyRainfall(), m_jun_rainfall_widget->getRainfallIntensity(),
                               m_dec_rainfall_widget->getMonthlyRainfall(), m_dec_rainfall_widget->getRainfallIntensity());
    hide();
    QDialog::accept();
}

void RainfallEditDialog::showEvent(QShowEvent * event)
{
    m_jun_rainfall_widget->push();
    m_dec_rainfall_widget->push();
}

void RainfallEditDialog::reset()
{
    m_jun_rainfall_widget->reset();
    m_dec_rainfall_widget->reset();
}
