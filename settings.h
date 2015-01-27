#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QFile>
#include <QDialog>
#include <QSlider>
#include <QSpinBox>

#define DEFAULT_TERRAIN_DIM 1000
#define DEFAULT_CAMERA_SENSITIVITY 5
#define DEFAULT_MOVEMENT_SENSITIVITY 5
struct Settings
{
public:
    int terrain_dimension;
    int camera_sensitivity;
    int z_movement_sensitivity;
    int x_y_movement_sensitivity;
    Settings() : terrain_dimension(DEFAULT_TERRAIN_DIM), camera_sensitivity(DEFAULT_CAMERA_SENSITIVITY),
        z_movement_sensitivity(DEFAULT_MOVEMENT_SENSITIVITY), x_y_movement_sensitivity(DEFAULT_MOVEMENT_SENSITIVITY) {}
};

class SettingsFile
{
public:
    SettingsFile();
    ~SettingsFile();
    Settings m_settings;
protected:

private:
};

class SettingsEditor : public QDialog
{
Q_OBJECT
public:
    SettingsEditor ( QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~SettingsEditor();
    Settings getSettings() { return m_settings_file.m_settings; }

protected:

private slots:
    void settings_changed();

private:
    void init_layout();
    void init_signals();
    SettingsFile m_settings_file;

    QSlider * m_camera_sensitivity_slider;
    QSlider * m_z_movement_sensitivity_slider;
    QSlider * m_x_y_movement_sensitivity_slider;
    QSpinBox * m_terrain_dim_spinbox;
};

#endif // SETTINGS_H
