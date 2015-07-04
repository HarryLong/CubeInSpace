#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QFile>
#include <QDialog>
#include <QSettings>
#include <QSlider>

class QLabel;
class QPushButton;
class QIntLineEdit;
class QCheckBox;
class SensitivitySlider : public QSlider
{
public:
    SensitivitySlider(QWidget * parent);
    static const int _DEFAULT;

};

struct Settings : public QObject
{
Q_OBJECT
public:
    Settings();
    ~Settings();

    int getRotationSensitivity() const;
    int getTranslationSensitivity() const;
    int getTerrainScale() const;
    bool useTerrainDefaultScale() const;

public slots:
    void setRotationSensitivity(int sensitivity);
    void setTranslationSensitivity(int sensitivity);
    void setTerrainScale(int scale);
    void setUseTerrainDefaultScale(bool use_default);

private:
    static const QString _key_rotation_sensitivity;
    static const QString _key_translation_sensitivity;
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

    SensitivitySlider * m_rotation_sensitivity_slider;
    SensitivitySlider * m_translation_sensitivity_slider;

    QIntLineEdit * m_terrain_scale_le;
    QCheckBox * m_default_scale_cb;

private slots:

private:
    void init_layout();
    void init_signals();

    Settings m_settings;
};

#endif // SETTINGS_H
