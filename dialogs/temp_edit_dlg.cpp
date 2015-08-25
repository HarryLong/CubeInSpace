#include "temp_edit_dlg.h"

#include <QLineEdit>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>

/*************************
 * TEMPERATURE LINE EDIT *
 *************************/
#define MIN_TEMP -50
#define MAX_TEMP 50
#define DEFAULT_TEMP 15
TemperatureLineEdit::TemperatureLineEdit(QWidget * parent) : QIntLineEdit(MIN_TEMP, MAX_TEMP, DEFAULT_TEMP, parent)
{

}

TemperatureLineEdit::~TemperatureLineEdit()
{

}

/*******************
 * LAPSE LINE EDIT *
 *******************/
#define MIN_LAPSE_RATE 0.0
#define MAX_LAPSE_RATE 50.0
#define DEFAULT_LAPSE_RATE 6.4
LapseLineEdit::LapseLineEdit(QWidget * parent) : QDoubleLineEdit(MIN_LAPSE_RATE, MAX_LAPSE_RATE, 2, DEFAULT_LAPSE_RATE, parent)
{
    setToolTip("Lapse rate is the temperature decrease, in degrees, for every km. of altitude gained (average: 6.4).");
}

LapseLineEdit::~LapseLineEdit()
{

}

/*****************************
 * TEMPERATURE EDITOR DIALOG *
 *****************************/
TemperatureEditDialog::TemperatureEditDialog(QWidget * parent, Qt::WindowFlags f ) :
    QDialog(parent,f), m_jun_temp_edit(new TemperatureLineEdit(this)), m_dec_temp_edit(new TemperatureLineEdit(this)),
    m_lapse_rate_edit(new LapseLineEdit(this)), m_ok_btn(new QPushButton("OK")), m_cancel_btn(new QPushButton("Cancel")),
    m_info_lbl(new QLabel(this))
{
    setWindowTitle("Temperature Editor");

    init_layout();

    connect(m_ok_btn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_cancel_btn, SIGNAL(clicked()), this, SLOT(reject()));
}

TemperatureEditDialog::~TemperatureEditDialog()
{

}

int TemperatureEditDialog::getJunTemp()
{
    return m_jun_temp_edit->value();
}

int TemperatureEditDialog::getDecTemp()
{
    return m_dec_temp_edit->value();
}

float TemperatureEditDialog::getLapseRate()
{
    return m_lapse_rate_edit->value();
}

void TemperatureEditDialog::init_layout()
{
    QVBoxLayout * main_layout (new QVBoxLayout);

    // June Temperature
    {
        main_layout->addWidget(new QLabel("<html> <span style=\"font-size:14pt; font-weight:600;\"> JUNE </span> </html>"),0, Qt::AlignCenter);

        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Temperature at zero meters: "),2, Qt::AlignLeft);
        layout->addWidget(m_jun_temp_edit, 1, Qt::AlignRight);

        main_layout->addLayout(layout,0);
    }

    // December Temperature
    {
        main_layout->addWidget(new QLabel("<html> <span style=\"font-size:14pt; font-weight:600;\"> DECEMBER </span> </html>"),0, Qt::AlignCenter);

        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Temperature at zero meters: "),2, Qt::AlignLeft);
        layout->addWidget(m_dec_temp_edit, 1, Qt::AlignRight);

        main_layout->addLayout(layout,0);
    }

    // Lapse Rate
    {
        main_layout->addWidget(new QLabel("<html> <span style=\"font-size:14pt; font-weight:600;\"> LAPSE RATE </span> </html>"),0, Qt::AlignCenter);

        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Lapse rate: "),2, Qt::AlignLeft);
        layout->addWidget(m_lapse_rate_edit, 1, Qt::AlignRight);

        main_layout->addLayout(layout);
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

void TemperatureEditDialog::reject()
{
    pop();
    QDialog::reject();
}

void TemperatureEditDialog::accept()
{
    QDialog::accept();
    emit temperatureValuesChanged(getJunTemp(), getDecTemp(), getLapseRate());
    hide();
}

void TemperatureEditDialog::showEvent(QShowEvent * event)
{
    push();
}

void TemperatureEditDialog::reset()
{
    m_jun_temp_edit->reset();
    m_dec_temp_edit->reset();
    m_lapse_rate_edit->reset();
}

void TemperatureEditDialog::push()
{
    m_cached_jun_temp = QString::number(m_jun_temp_edit->value());
    m_cached_dec_temp = QString::number(m_dec_temp_edit->value());
    m_cached_lapse_rate = QString::number(m_lapse_rate_edit->value());
}

void TemperatureEditDialog::pop()
{
    m_jun_temp_edit->setText(m_cached_jun_temp);
    m_dec_temp_edit->setText(m_cached_dec_temp);
    m_lapse_rate_edit->setText(m_cached_lapse_rate);
}
