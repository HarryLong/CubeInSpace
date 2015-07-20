#ifndef LATITUDE_CONTROLLER_H
#define LATITUDE_CONTROLLER_H

#include "base_slider_controller.h"

/******************************
 * LATITUDE CONTROLLER WIDGET *
 ******************************/
class LatitudeSlider : public QSlider
{
public:
    LatitudeSlider(QWidget * parent = 0);
    ~LatitudeSlider();
};
class LatitudeControllerWidget : public BaseSliderControllerWidget
{
    Q_OBJECT
public:
    LatitudeControllerWidget(int alignment, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~LatitudeControllerWidget();
    int getLatitude() const;

protected slots:
    virtual void refresh_label(int value);
};

#endif // LATITUDE_CONTROLLER_H
