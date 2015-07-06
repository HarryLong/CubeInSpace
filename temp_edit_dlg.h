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
    bool valid();
    void reset();

    void push();
    void pop();

signals:
    void text_changed();

private slots:
    void emit_text_changed();

private:
    void init_layout();

    TemperatureLineEdit * m_temp_le;
    LapseLineEdit * m_lapse_rate_le;

    QString m_cached_temp, m_cached_lapse;
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
    TemperatureAttributes getJunTemperatureAttributes();
    TemperatureAttributes getDecTemperatureAttributes();

    QPushButton * m_ok_btn;

signals:    
    void temperatureValuesChanged(float temp_at_zero_june, float lapse_rate_june, float temp_at_zero_dec, float lapse_rate_dec);

public slots:
    virtual void reject();
    virtual void accept();

private slots:
    void perform_input_validity_check();

protected:
    virtual void showEvent(QShowEvent * event);

private:
    void init_layout();

    QPushButton * m_cancel_btn;
    TemperatureEditWidget * m_jun_temp_edit;
    TemperatureEditWidget * m_dec_temp_edit;
    QLabel * m_info_lbl;
};
#endif // TEMP_EDIT_DIALOG_H
