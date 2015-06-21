#include "custom_line_edit.h"
#include <algorithm>

/********************
 * CUSTOM LINE EDIT *
 ********************/
CustomLineEdit::CustomLineEdit(QWidget * parent) : QLineEdit(parent)
{
    m_valid_palette.setColor(QPalette::Base, Qt::white);
    m_invalid_palette.setColor(QPalette::Base, Qt::red);

    connect(this, SIGNAL(textChanged(QString)), this, SLOT(checkValidity()));

    checkValidity();
}

CustomLineEdit::~CustomLineEdit()
{

}

void CustomLineEdit::setValid(bool valid)
{
    setPalette(valid ? m_valid_palette : m_invalid_palette);
}

bool CustomLineEdit::valid()
{
    return text().length() != 0;
}

void CustomLineEdit::checkValidity()
{
    if(valid())
    {
        setValid(true);
        emit inputValid();
    }
    else
    {
        setValid(false);
        emit inputInvalid();
    }
}

/********************
 * DOUBLE LINE EDIT *
 ********************/
#include <QDoubleValidator>
QDoubleLineEdit::QDoubleLineEdit(double min, double max, int decimals, QWidget * parent) : CustomLineEdit(parent), m_validator(new QDoubleValidator(min, max, decimals, this))
{
    m_validator->setNotation(QDoubleValidator::StandardNotation);
    setValidator(m_validator);

    connect(this, SIGNAL(textChanged(QString)), this, SLOT(emit_value_changed()));
}

QDoubleLineEdit::~QDoubleLineEdit()
{

}

void QDoubleLineEdit::setRange(double min, double max, int decimals)
{
    m_validator->setRange(min, max, decimals);
}

double QDoubleLineEdit::value()
{
    return std::max(m_validator->bottom(), std::min(m_validator->top(),text().toDouble()));
}

void QDoubleLineEdit::emit_value_changed()
{
    emit valueChanged(value());
}

/*****************
 * INT LINE EDIT *
 *****************/
#include <QIntValidator>
QIntLineEdit::QIntLineEdit(int min, int max, QWidget * parent) : CustomLineEdit(parent), m_validator(new QIntValidator(min, max, this))
{
    setValidator(m_validator);

    connect(this, SIGNAL(textChanged(QString)), this, SLOT(emit_value_changed()));
}

QIntLineEdit::~QIntLineEdit()
{
    delete m_validator;
}

void QIntLineEdit::setRange(int min, int max)
{
    m_validator->setRange(min, max);
    setValidator(m_validator);
}

int QIntLineEdit::value()
{
    return std::max(m_validator->bottom(), std::min(m_validator->top(),text().toInt()));
}

void QIntLineEdit::emit_value_changed()
{
    emit valueChanged(value());
}
