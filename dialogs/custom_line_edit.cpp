#include "custom_line_edit.h"
#include <algorithm>

/********************
 * DOUBLE LINE EDIT *
 ********************/
#include <QDoubleValidator>
QDoubleLineEdit::QDoubleLineEdit(double min, double max, int decimals, double default_value, QWidget * parent) :
    QLineEdit(parent), m_validator(new QDoubleValidator(min, max, decimals, this)), m_default(default_value)
{
    reset();
    m_validator->setNotation(QDoubleValidator::StandardNotation);
    setValidator(m_validator);
}

QDoubleLineEdit::~QDoubleLineEdit()
{

}

void QDoubleLineEdit::focusOutEvent(QFocusEvent * e)
{
    if(text().isEmpty())
        setText(QString::number(m_default));

    QLineEdit::focusOutEvent(e);
}

void QDoubleLineEdit::setRange(double min, double max, int decimals)
{
    m_validator->setRange(min, max, decimals);
}

double QDoubleLineEdit::value()
{
    return std::max(m_validator->bottom(), std::min(m_validator->top(),text().toDouble()));
}

void QDoubleLineEdit::reset()
{
    setText(QString::number(m_default));
}

/*****************
 * INT LINE EDIT *
 *****************/
#include <QIntValidator>
QIntLineEdit::QIntLineEdit(int min, int max, int default_value, QWidget * parent) :
    QLineEdit(parent), m_validator(new QIntValidator(min, max, this)), m_default(default_value)
{
    reset();
    setValidator(m_validator);
}

QIntLineEdit::~QIntLineEdit()
{
    delete m_validator;
}

void QIntLineEdit::focusOutEvent(QFocusEvent * e)
{
    if(text().isEmpty())
        setText(QString::number(m_default));

    QLineEdit::focusOutEvent(e);
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

void QIntLineEdit::reset()
{
    setText(QString::number(m_default));
}
