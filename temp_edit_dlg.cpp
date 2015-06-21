#include "temp_edit_dlg.h"

#include <QLineEdit>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>

/*************************
 * TEMPERATURE LINE EDIT *
 *************************/
#define MIN_TEMP -50.0
#define MAX_TEMP 50.0
TemperatureLineEdit::TemperatureLineEdit(QWidget * parent) : QDoubleLineEdit(MIN_TEMP, MAX_TEMP, 2, parent)
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
LapseLineEdit::LapseLineEdit(QWidget * parent) : QDoubleLineEdit(MIN_LAPSE_RATE, MAX_LAPSE_RATE, 2, parent)
{
    setToolTip("Lapse rate is the temperature decrease, in degrees, for every km. of altitude gained (average: 6.4).");
}

LapseLineEdit::~LapseLineEdit()
{

}

/*****************************
 * TEMPERATURE EDITOR WIDGET *
 *****************************/
TemperatureEditWidget::TemperatureEditWidget(QWidget * parent, Qt::WindowFlags f) :
    QWidget(parent, f), m_temp_le(new TemperatureLineEdit(this)), m_lapse_rate_le(new LapseLineEdit(this)), m_temp_valid(false)
{
    init_layout();
    connect(m_temp_le, SIGNAL(inputValid()), this, SLOT(emit_valid_input_signal()));
    connect(m_temp_le, SIGNAL(inputInvalid()), this, SLOT(emit_invalid_input_signal()));

    connect(m_lapse_rate_le, SIGNAL(inputValid()), this, SLOT(emit_valid_input_signal()));
    connect(m_lapse_rate_le, SIGNAL(inputInvalid()), this, SLOT(emit_invalid_input_signal()));
}

TemperatureEditWidget::~TemperatureEditWidget()
{

}

void TemperatureEditWidget::init_layout()
{
    QVBoxLayout * main_layout (new QVBoxLayout);

    // Temperature
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Temperature at zero meters: "),2, Qt::AlignLeft);
        layout->addWidget(m_temp_le, 1, Qt::AlignRight);

        main_layout->addLayout(layout);
    }

    // Lapse
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Lapse rate: "),2, Qt::AlignLeft);
        layout->addWidget(m_lapse_rate_le, 1, Qt::AlignRight);

        main_layout->addLayout(layout);
    }

    setLayout(main_layout);
}

float TemperatureEditWidget::getTemperature()
{
    return m_temp_le->value();
}

float TemperatureEditWidget::getLapseRate()
{
    return m_lapse_rate_le->value();
}

bool TemperatureEditWidget::isValid()
{
    return m_temp_valid && m_temp_le->valid() && m_lapse_rate_le->valid();
}

void TemperatureEditWidget::reset()
{
    m_temp_le->setText("");
    m_lapse_rate_le->setText("");
}

void TemperatureEditWidget::pop()
{
    m_temp_le->setText(m_cached_temp);
    m_lapse_rate_le->setText(m_cached_lapse);
}

void TemperatureEditWidget::push()
{
    m_cached_temp = m_temp_le->text();
    m_cached_lapse = m_lapse_rate_le->text();
}

void TemperatureEditWidget::setTemperatureValid(bool valid)
{
    m_temp_valid = valid;
    m_temp_le->setValid(isValid());
}

void TemperatureEditWidget::emit_valid_input_signal()
{
    emit valid_input();
}

void TemperatureEditWidget::emit_invalid_input_signal()
{
    emit invalid_input();
}

/*****************************
 * TEMPERATURE EDITOR DIALOG *
 *****************************/
TemperatureEditDialog::TemperatureEditDialog(QWidget * parent, Qt::WindowFlags f ) :
    QDialog(parent,f), m_min_temp_edit(new TemperatureEditWidget), m_max_temp_edit(new TemperatureEditWidget),
    m_ok_btn(new QPushButton("OK")), m_cancel_btn(new QPushButton("Cancel")), m_info_lbl(new QLabel(this))
{
    setWindowTitle("Temperature Editor");

    init_layout();

    connect(m_ok_btn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_cancel_btn, SIGNAL(clicked()), this, SLOT(reject()));

    connect(m_min_temp_edit, SIGNAL(invalid_input()), this, SLOT(invalid()));
    connect(m_max_temp_edit, SIGNAL(invalid_input()), this, SLOT(invalid()));

    connect(m_min_temp_edit, SIGNAL(valid_input()), this, SLOT(perform_input_validity_check()));
    connect(m_max_temp_edit, SIGNAL(valid_input()), this, SLOT(perform_input_validity_check()));

    perform_input_validity_check();
}

TemperatureEditDialog::~TemperatureEditDialog()
{

}

bool TemperatureEditDialog::isValid()
{
    return m_min_temp_edit->isValid() && m_max_temp_edit->isValid();
}

TemperatureAttributes TemperatureEditDialog::getMinTemperatureAttributes()
{
    return TemperatureAttributes(m_min_temp_edit->getTemperature(), m_min_temp_edit->getLapseRate());
}

TemperatureAttributes TemperatureEditDialog::getMaxTemperatureAttributes()
{
    return TemperatureAttributes(m_max_temp_edit->getTemperature(), m_max_temp_edit->getLapseRate());
}

void TemperatureEditDialog::init_layout()
{
    QVBoxLayout * main_layout (new QVBoxLayout);

    // Minimum temperature
    {
        main_layout->addWidget(new QLabel("<html> <span style=\"font-size:14pt; font-weight:600;\"> MINIMUM (WINTER) </span> </html>"),0, Qt::AlignCenter);
        main_layout->addWidget(m_min_temp_edit,0);
    }

    // Maximum temperature
    {
        main_layout->addWidget(new QLabel("<html> <span style=\"font-size:14pt; font-weight:600;\"> MAXIMUM (SUMMER) </span> </html>"),0, Qt::AlignCenter);
        main_layout->addWidget(m_max_temp_edit,0);
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
    m_min_temp_edit->pop();
    m_max_temp_edit->pop();

    QDialog::reject();
}

void TemperatureEditDialog::accept()
{
    emit temperatureValuesChanged(m_min_temp_edit->getTemperature(), m_min_temp_edit->getLapseRate(),
                                  m_max_temp_edit->getTemperature(), m_max_temp_edit->getLapseRate());
    QDialog::reject();
}

void TemperatureEditDialog::showEvent(QShowEvent * event)
{
    m_min_temp_edit->push();
    m_max_temp_edit->push();
}

void TemperatureEditDialog::reset()
{
    m_min_temp_edit->reset();
    m_max_temp_edit->reset();
}

void TemperatureEditDialog::invalid()
{
    m_ok_btn->setEnabled(false);
}

void TemperatureEditDialog::perform_input_validity_check()
{
    bool temp_check (m_min_temp_edit->getTemperature() <= m_max_temp_edit->getTemperature());
    if(!temp_check)
    {
        m_info_lbl->setText("<html> <span style=\"color:red;\">! Min temp > Max temp !</span> </html>");
    }
    else
    {
        m_info_lbl->setText(" ");
    }

    m_min_temp_edit->setTemperatureValid(temp_check);
    m_max_temp_edit->setTemperatureValid(temp_check);

    m_ok_btn->setEnabled(m_min_temp_edit->isValid() && m_max_temp_edit->isValid());
}
