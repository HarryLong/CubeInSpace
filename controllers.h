#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include <QSlider>
#include <QCheckBox>
#include "custom_line_edit.h"
#include "QAction"
#include <QPushButton>
struct PositionControllers{
public:
    PositionControllers(QSlider * latitude_slider) : latitude_slider(latitude_slider) {}
    QSlider * latitude_slider;
};
struct TimeControllers{
public:
    TimeControllers(QSlider * time_of_day_slider, QSlider * month_slider) : time_of_day_slider(time_of_day_slider), month_slider(month_slider) {}

    QSlider * time_of_day_slider;
    QSlider * month_slider;
};
struct ViewControllers{
public:
    ViewControllers(QSlider * translation_sensitivity, QSlider * rotation_sensitivity) :
        translation_sensitivity(translation_sensitivity), rotation_sensitivity(rotation_sensitivity)  {}

    QSlider * translation_sensitivity;
    QSlider * rotation_sensitivity;
};

#endif // CONTROLLERS_H
