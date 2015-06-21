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
    ViewControllers(QSlider * camera_sensitivity_slider, QSlider * z_movement_sensitivity_slider, QSlider * x_y_movement_sensitivity_slider) :
        camera_sensitivity_slider(camera_sensitivity_slider), z_movement_sensitivity_slider(z_movement_sensitivity_slider),
        x_y_movement_sensitivity_slider(x_y_movement_sensitivity_slider) {}

    QSlider * camera_sensitivity_slider;
    QSlider * z_movement_sensitivity_slider;
    QSlider * x_y_movement_sensitivity_slider;
};
struct TerrainControllers{
public:
    TerrainControllers(QIntLineEdit * terrain_scale_le, QCheckBox * use_default_cb) :
        terrain_scale_le(terrain_scale_le), use_default_scale_cb(use_default_cb) {}

    QIntLineEdit * terrain_scale_le;
    QCheckBox * use_default_scale_cb;
};

#endif // CONTROLLERS_H
