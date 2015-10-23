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
class QButtonGroup;
class SensitivitySlider : public QSlider
{
public:
    SensitivitySlider(int value, QWidget * parent);
    ~SensitivitySlider();

    void pop();
    void push();

    static const int _DEFAULT;
private:
    int m_pushed_value;
};

class QRadioButton;
class ControlModeSelectionWidget : public QWidget
{
Q_OBJECT
public:
    static const enum Mode{
        _FPS = 0,
        _SOFTIMAGE
    } _DEFAULT = Mode::_FPS;

    ControlModeSelectionWidget(Mode mode, QWidget * parent);
    ~ControlModeSelectionWidget();

    Mode getActiveControlMode();

    void push();

public slots:
    void pop();

private slots:
    void setMode(Mode mode);

private:
    QRadioButton * m_fps_btn;
    QRadioButton * m_softimage_btn;

    Mode m_cached_mode;

    void init_layout();
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
    ControlModeSelectionWidget::Mode getControlMode();
    bool useTerrainDefaultScale() const;

public slots:
    void setRotationSensitivity(int sensitivity);
    void setTranslationSensitivity(int sensitivity);
    void setControlMode(ControlModeSelectionWidget::Mode mode);

private:
    static const QString _key_rotation_sensitivity;
    static const QString _key_translation_sensitivity;
    static const QString _key_control_mode;

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
    bool fps();
    bool softimage();
    ControlModeSelectionWidget::Mode getControlMode();

    static QString get_two_number_digit(int digit);

private slots:
    void save();
    void dismiss();

private:
    SensitivitySlider * m_rotation_sensitivity_slider;
    SensitivitySlider * m_translation_sensitivity_slider;
    ControlModeSelectionWidget * m_control_mode_selection_widget;
    void init_layout();
    void init_signals();
    QPushButton * m_ok_btn;
    QPushButton * m_cancel_btn;

    Settings m_settings;
};

#endif // SETTINGS_H
