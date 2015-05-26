#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QFile>
#include <QDialog>
#include <QSlider>
#include <QComboBox>

#define DEFAULT_TERRAIN_SCALE 1
#define DEFAULT_CAMERA_SENSITIVITY 5
#define DEFAULT_MOVEMENT_SENSITIVITY 5
struct Settings
{
public:
    int terrain_scaler;
    int camera_sensitivity;
    int z_movement_sensitivity;
    int x_y_movement_sensitivity;
    Settings() : terrain_scaler(DEFAULT_TERRAIN_SCALE), camera_sensitivity(DEFAULT_CAMERA_SENSITIVITY),
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

class SettingsEditorDialog : public QDialog
{
Q_OBJECT
public:
    SettingsEditorDialog ( QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~SettingsEditorDialog();
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
    QComboBox * m_terrain_scaler_cb;
};

#endif // SETTINGS_H
