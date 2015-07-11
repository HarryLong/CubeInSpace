#ifndef RAINFALL_EDIT_DIALOG_H
#define RAINFALL_EDIT_DIALOG_H

#include <QDialog>
#include "custom_line_edit.h"

/********************************
 * RAINFALL INTENSITY LINE EDIT *
 ********************************/
class RainfallIntensityLineEdit : public QIntLineEdit
{
public:
    RainfallIntensityLineEdit(QWidget * parent = 0);
    ~RainfallIntensityLineEdit();
};

/**********************
 * RAINFALL LINE EDIT *
 **********************/
class MonthlyRainfallLineEdit : public QIntLineEdit
{
public:
    MonthlyRainfallLineEdit(QWidget * parent = 0);
    ~MonthlyRainfallLineEdit();
};

/*****************************
 * TEMPERATURE EDITOR WIDGET *
 *****************************/
class QLabel;
class QSlider;
class QPushButton;
class RainfallEditWidget : public QWidget{
    Q_OBJECT
public:
    RainfallEditWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~RainfallEditWidget();

    int getMonthlyRainfall();
    int getRainfallIntensity();
    void reset();

    void push();
    void pop();

private:
    void init_layout();

    MonthlyRainfallLineEdit * m_monthly_rainfall_le;
    RainfallIntensityLineEdit * m_rainfall_intensity_le;

    QString m_cached_monthly_rainfall, m_cached_rainfall_intensity;
};

/*****************************
 * TEMPERATURE EDITOR DIALOG *
 *****************************/
class RainfallEditDialog : public QDialog {
    Q_OBJECT
public:
    struct RainfallAttributes{
    public:
        /**
         * @brief RainfallAttributes
         * @param monthly_rainfall: in mm
         * @param avg_rainfall_intensity: in mm/h
         */
        RainfallAttributes(int monthly_rainfall, int rainfall_intensity) :
            monthly_rainfall(monthly_rainfall), rainfall_intensity(rainfall_intensity) { }

        int monthly_rainfall;
        int rainfall_intensity;
    };

    RainfallEditDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~RainfallEditDialog();

    bool isValid();
    void reset();
    RainfallAttributes getJunRainfallAttributes();
    RainfallAttributes getDecRainfallAttributes();

    QPushButton * m_ok_btn;

signals:
    void rainfallValuesChanged(int rainfall_jun, int rainfal_intensity_jun, int rainfall_dec, int rainfal_intensity_dec);

public slots:
    virtual void reject();
    virtual void accept();

protected:
    virtual void showEvent(QShowEvent * event);

private:
    void init_layout();

    QPushButton * m_cancel_btn;
    RainfallEditWidget * m_jun_rainfall_widget;
    RainfallEditWidget * m_dec_rainfall_widget;
    QLabel * m_info_lbl;
};

#endif // RAINFALL_EDIT_DIALOG_H
