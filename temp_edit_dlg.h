#ifndef TEMP_EDIT_DIALOG_H
#define TEMP_EDIT_DIALOG_H

#include <QDialog>
#include "custom_line_edit.h"

/*************************
 * TEMPERATURE LINE EDIT *
 *************************/
class TemperatureLineEdit : public QDoubleLineEdit
{
public:
    TemperatureLineEdit(QWidget * parent = 0);
    ~TemperatureLineEdit();
};

/*******************
 * LAPSE LINE EDIT *
 *******************/
class LapseLineEdit : public QDoubleLineEdit
{
public:
    LapseLineEdit(QWidget * parent = 0);
    ~LapseLineEdit();
};

/*****************************
 * TEMPERATURE EDITOR WIDGET *
 *****************************/
class QLabel;
class QDoubleLineEdit;
class QSlider;
class QPushButton;
class TemperatureEditWidget : public QWidget{
    Q_OBJECT
public:
    TemperatureEditWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~TemperatureEditWidget();

    float getTemperature();
    float getLapseRate();
    bool isValid();
    void reset();

    void push();
    void pop();

public slots:
    void setTemperatureValid(bool valid);

signals:
    void valid_input();
    void invalid_input();

private slots:
    void emit_valid_input_signal();
    void emit_invalid_input_signal();

private:
    void init_layout();

    TemperatureLineEdit * m_temp_le;
    LapseLineEdit * m_lapse_rate_le;

    QString m_cached_temp, m_cached_lapse;

    bool m_temp_valid;
};

struct TemperatureAttributes{
public:
    TemperatureAttributes( float temperature_at_zero_meters, float lapse_rate) :
        temperature_at_zero_meters(temperature_at_zero_meters), lapse_rate(lapse_rate) { }

    float temperature_at_zero_meters;
    float lapse_rate;
};

/*****************************
 * TEMPERATURE EDITOR DIALOG *
 *****************************/
class TemperatureEditDialog : public QDialog {
    Q_OBJECT
public:
    TemperatureEditDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~TemperatureEditDialog();

    bool isValid();
    void reset();
    TemperatureAttributes getMinTemperatureAttributes();
    TemperatureAttributes getMaxTemperatureAttributes();

    QPushButton * m_ok_btn;

signals:
    void temperatureValuesChanged(float min_at_zero, float min_lapse_rate, float max_at_zero, float max_lapse_rate);

public slots:
    virtual void reject();
    virtual void accept();

private slots:
    void perform_input_validity_check();
    void invalid();

protected:
    virtual void showEvent(QShowEvent * event);

private:
    void init_layout();

    QPushButton * m_cancel_btn;
    TemperatureEditWidget * m_min_temp_edit;
    TemperatureEditWidget * m_max_temp_edit;
    QLabel * m_info_lbl;
};
#endif // TEMP_EDIT_DIALOG_H
