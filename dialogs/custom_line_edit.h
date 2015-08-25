#ifndef CUSTOM_LINE_EDIT_H
#define CUSTOM_LINE_EDIT_H

#include <QLineEdit>

/********************
 * DOUBLE LINE EDIT *
 ********************/
class QDoubleValidator;
class QDoubleLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    QDoubleLineEdit(double min, double max, int decimals, double default_value, QWidget * parent = 0);
    ~QDoubleLineEdit();
    double value();
    void setValue(double value);
    void reset();

public slots:
    void setRange(double min, double max, int decimals);

protected:
    virtual void focusOutEvent(QFocusEvent * e) override;

private:
    QDoubleValidator * m_validator;
    double m_default;
};

/*****************
 * INT LINE EDIT *
 *****************/
class QIntValidator;
class QIntLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    QIntLineEdit(int min, int max, int default_value, QWidget * parent = 0);
    ~QIntLineEdit();

    int value();
    void setValue(int value);
    void reset();

public slots:
    void setRange(int min, int max);

protected:
    virtual void focusOutEvent(QFocusEvent * e) override;

private:
    QIntValidator * m_validator;
    int m_default;
};

#endif // CUSTOM_LINE_EDIT_H
