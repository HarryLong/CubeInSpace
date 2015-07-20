#ifndef SOIL_INFILTRATION_CONTROLLER_H
#define SOIL_INFILTRATION_CONTROLLER_H

#include "base_slider_controller.h"

/*************************************
 * SOIL ABSORPTION CONTROLLER WIDGET *
 *************************************/
class SoilInfiltrationSlider : public QSlider
{
public:
    SoilInfiltrationSlider(QWidget * parent = 0);
    ~SoilInfiltrationSlider();
};

class SoilInfiltrationControllerWidget : public BaseSliderControllerWidget
{
    Q_OBJECT
public:
    SoilInfiltrationControllerWidget(int alignment, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~SoilInfiltrationControllerWidget();
    int getInfiltration() const;

    static const float _MAX_INFILTRATION_RATE;

protected slots:
    virtual void refresh_label(int value);
};

#endif //SOIL_INFILTRATION_CONTROLLER_H
