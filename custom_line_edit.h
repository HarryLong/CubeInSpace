#ifndef CUSTOM_LINE_EDIT_H
#define CUSTOM_LINE_EDIT_H

#include <QLineEdit>

/********************
 * CUSTOM LINE EDIT *
 ********************/
class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    virtual ~CustomLineEdit();
    void setValid(bool valid);
    virtual bool valid();

signals:
    void inputValid();
    void inputInvalid();

public slots:
    virtual void checkValidity();

protected:
    CustomLineEdit(QWidget * parent);

private:
    QPalette m_valid_palette;
    QPalette m_invalid_palette;
};

/********************
 * DOUBLE LINE EDIT *
 ********************/
class QDoubleValidator;
class QDoubleLineEdit : public CustomLineEdit
{
    Q_OBJECT
public:
    QDoubleLineEdit(double min, double max, int decimals, QWidget * parent = 0);
    ~QDoubleLineEdit();
    double value();

signals:
    void valueChanged(double);

public slots:
    void setRange(double min, double max, int decimals);

private slots:
    void emit_value_changed();

private:
    QDoubleValidator * m_validator;
};

/*****************
 * INT LINE EDIT *
 *****************/
class QIntValidator;
class QIntLineEdit : public CustomLineEdit
{
    Q_OBJECT
public:
    QIntLineEdit(int min, int max, QWidget * parent = 0);
    ~QIntLineEdit();

    int value();

signals:
    void valueChanged(int);

public slots:
    void setRange(int min, int max);

private slots:
    void emit_value_changed();

private:
    QIntValidator * m_validator;
};

#endif // CUSTOM_LINE_EDIT_H
