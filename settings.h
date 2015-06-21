#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QFile>
#include <QDialog>
#include <QSettings>
class QSlider;
class QLabel;
class QPushButton;
class QIntLineEdit;
class QCheckBox;
struct Settings : public QObject
{
Q_OBJECT
public:
    Settings();
    ~Settings();

    int getCameraSensitivity() const;
    int getZSensitivity() const;
    int getXYSensitivity() const;
    int getTerrainScale() const;
    bool useTerrainDefaultScale() const;

public slots:
    void setCameraSensitivity(int sensitivity);
    void setZSensitivity(int sensitivity);
    void setXYSensitivity(int sensitivity);
    void setTerrainScale(int scale);
    void setUseTerrainDefaultScale(bool use_default);

private:
    static const QString _key_camera_sensitivity;
    static const QString _key_z_sensitivity;
    static const QString _key_x_y_sensitivity;
    static const QString _key_terrain_scale;
    static const QString _key_use_terrain_default_scale;

    static const int _default_camera_sensitivity;
    static const int _default_z_sensitivity;
    static const int _default_x_y_sensitivity;
    static const int _default_scale;
    static const bool _default_use_terrain_default_scale;

    QSettings m_core_settings;
};

class SettingsDialog : public QDialog
{
Q_OBJECT
public:
    SettingsDialog ( QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~SettingsDialog();
    const Settings & getSettings() { return m_settings; }

    virtual QSize sizeHint() const;

    static QString get_two_number_digit(int digit);

    QSlider * m_camera_sensitivity_slider;
    QSlider * m_z_movement_sensitivity_slider;
    QSlider * m_x_y_movement_sensitivity_slider;

    QIntLineEdit * m_terrain_scale_le;
    QCheckBox * m_default_scale_cb;

private slots:

private:
    void init_layout();
    void init_signals();

    Settings m_settings;

};

#endif // SETTINGS_H
