#ifndef TIME_CONTROLLER_DIALOG_H
#define TIME_CONTROLLER_DIALOG_H

#include <QDialog>
#include <QSlider>
#include <QLabel>

static QString g_months [] {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
};

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

    TimeOfDaySlider * m_time_of_day_slider;
    MonthOfYearSlider * m_month_of_year_slider;

    QLabel * m_month_lbl;
    QLabel * m_time_lbl;

protected:
    QSize sizeHint() const;//Override

private slots:
    void refresh_labels();

private:
    void init_layout();
    QString format_time(int minutes);
    QString get_two_number_digit(int digit);
};

#endif // TIME_CONTROLLER_DIALOG_H
