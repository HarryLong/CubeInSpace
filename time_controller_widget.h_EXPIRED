#ifndef TIME_CONTROLLER_WIDGET_H
#define TIME_CONTROLLER_WIDGET_H

#include <QWidget>
#include <QSlider>

class MonthSlider : public QSlider
{
public:
    MonthSlider(QWidget * parent = 0);
};

class TimeSlider : public QSlider
{
public:
    TimeSlider(QWidget * parent = 0);
};

class QLabel;
class TimeControllerWidget : public QWidget
{
    Q_OBJECT
public:
    TimeControllerWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~TimeControllerWidget();

    int getTimeOfDay() const;
    int getMonthOfYear() const;

    TimeSlider * m_time_slider;
    MonthSlider * m_month_slider;

private slots:
    void refresh_label();

private:
    void init_layout();
    QLabel * m_lbl;
};


#endif // TIME_CONTROLLER_WIDGET_H
