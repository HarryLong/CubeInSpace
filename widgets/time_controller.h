#ifndef TIME_CONTROLLER_H
#define TIME_CONTROLLER_H

#include "base_slider_controller.h"
#include <QString>

/**************************
 * TIME CONTROLLER WIDGET *
 **************************/
class TimeSlider : public QSlider
{
public:
    TimeSlider(QWidget * parent = 0);
    ~TimeSlider();
};
class TimeControllerWidget : public BaseSliderControllerWidget
{
    Q_OBJECT
public:
    TimeControllerWidget(int alignment, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~TimeControllerWidget();
    int getTime() const;

protected slots:
    virtual void refresh_label(int value);

private:
    QString format_time(int time_in_minutes);
};

#endif // TIME_CONTROLLER_H
