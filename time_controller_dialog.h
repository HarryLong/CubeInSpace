#ifndef TIME_CONTROLLER_DIALOG_H
#define TIME_CONTROLLER_DIALOG_H

#include <QDialog>
#include <QSlider>
#include <QLabel>

class MonthOfYearSlider : public QSlider
{
public:
    MonthOfYearSlider();
};

class TimeOfDaySlider : public QSlider
{
public:
    TimeOfDaySlider();
};

class TimeControllerDialog : public QDialog {
    Q_OBJECT
public:
    TimeControllerDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~TimeControllerDialog();

    int getTimeOfDay();
    int getMonthOfYear();

    QSlider * m_time_of_day_slider;
    QSlider * m_month_of_year_slider;

    QLabel * m_month_lbl;
    QLabel * m_hour_lbl;

protected:
    QSize sizeHint() const;//Override

private slots:
    void refresh_labels();

private:
    void init_layout();
};

#endif // TIME_CONTROLLER_DIALOG_H
