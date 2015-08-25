#ifndef TEMP_EDIT_DIALOG_H
#define TEMP_EDIT_DIALOG_H

#include <QDialog>
#include "custom_line_edit.h"
#include <QLabel>

/*************************
 * TEMPERATURE LINE EDIT *
 *************************/
class TemperatureLineEdit : public QIntLineEdit
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
 * TEMPERATURE EDITOR DIALOG *
 *****************************/
class TemperatureEditDialog : public QDialog {
    Q_OBJECT
public:
    TemperatureEditDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~TemperatureEditDialog();

    void reset();

    int getJunTemp();
    int getDecTemp();
    float getLapseRate();

    QPushButton * m_ok_btn;

signals:    
    void temperatureValuesChanged(int temp_at_zero_june, int temp_at_zero_dec, float lapse_rate);

public slots:
    virtual void reject();
    virtual void accept();

protected:
    virtual void showEvent(QShowEvent * event);

private:
    void init_layout();
    void push();
    void pop();

    QString m_cached_jun_temp, m_cached_dec_temp, m_cached_lapse_rate;

    QPushButton * m_cancel_btn;
    TemperatureLineEdit * m_jun_temp_edit;
    TemperatureLineEdit * m_dec_temp_edit;
    LapseLineEdit * m_lapse_rate_edit;

    QLabel * m_info_lbl;
};
#endif // TEMP_EDIT_DIALOG_H
