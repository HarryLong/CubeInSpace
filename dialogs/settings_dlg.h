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

private:
    static const QString _key_rotation_sensitivity;
    static const QString _key_translation_sensitivity;

    static const int _default_camera_sensitivity;
    static const int _default_z_sensitivity;
    static const int _default_x_y_sensitivity;

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

    float getRotationSensitivity();
    float getTranslationSensitivity();

    static QString get_two_number_digit(int digit);

signals:
    void translationSensitivityChanged(float sensitivity);
    void rotationSensitivityChanged(float sensitivity);

private slots:
    void emit_translation_sensitivity_changed();
    void emit_rotation_sensitivity_changed();

private:
    QSlider * m_rotation_sensitivity_slider;
    QSlider * m_translation_sensitivity_slider;
    void init_layout();
    void init_signals();

    Settings m_settings;
};

#endif // SETTINGS_H
