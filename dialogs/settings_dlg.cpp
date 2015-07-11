#include "settings_dlg.h"
#include <QTextStream>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <iostream>
#include <QLabel>
#include <QCoreApplication>
#include <QPushButton>
#include <QCheckBox>
#include "custom_line_edit.h"

const int SensitivitySlider::_DEFAULT = 50;
SensitivitySlider::SensitivitySlider(QWidget * parent) : QSlider(Qt::Orientation::Horizontal, parent)
{
    setRange(0,100);
    setValue(SensitivitySlider::_DEFAULT);
}


// Keys
const QString Settings::_key_rotation_sensitivity ("rotation_sensitivity");
const QString Settings::_key_translation_sensitivity ("translation_sensitivity");
Settings::Settings() : m_core_settings(QCoreApplication::organizationName(), QCoreApplication::applicationName())
{

}

Settings::~Settings()
{

}

void Settings::setRotationSensitivity(int sensitivity)
{
    m_core_settings.setValue(Settings::_key_rotation_sensitivity, sensitivity);
    m_core_settings.sync();
}

void Settings::setTranslationSensitivity(int sensitivity)
{
    m_core_settings.setValue(Settings::_key_translation_sensitivity, sensitivity);
    m_core_settings.sync();
}

int Settings::getRotationSensitivity() const
{
    return m_core_settings.value(Settings::_key_rotation_sensitivity, SensitivitySlider::_DEFAULT).toInt();
}

int Settings::getTranslationSensitivity() const
{
    return m_core_settings.value(Settings::_key_translation_sensitivity, SensitivitySlider::_DEFAULT).toInt();
}

/*******************
 * SETTINGS EDITOR *
 *******************/
SettingsDialog::SettingsDialog ( QWidget * parent, Qt::WindowFlags f) : QDialog(parent,f),
    m_rotation_sensitivity_slider(new SensitivitySlider(parent)),
    m_translation_sensitivity_slider(new SensitivitySlider(parent))
{
    m_rotation_sensitivity_slider->setValue(m_settings.getRotationSensitivity());
    m_translation_sensitivity_slider->setValue(m_settings.getTranslationSensitivity());

    setModal(true);
    setWindowTitle("Settings");
    init_layout();
    init_signals();
}

SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::init_layout()
{
    QVBoxLayout * main_layout = new QVBoxLayout;
    // Rotation sensitivity
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->addWidget(new QLabel("Rotation sensitivity: "));
        layout->addWidget(m_rotation_sensitivity_slider,1);
        main_layout->addLayout(layout);
    }

    // Translation sensitivity
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->addWidget(new QLabel("Translation sensitivity "));
        layout->addWidget(m_translation_sensitivity_slider,1);
        main_layout->addLayout(layout);
    }

    main_layout->addWidget(new QLabel(""), 1); // Padding

    setLayout(main_layout);
}

void SettingsDialog::init_signals()
{
    connect(m_rotation_sensitivity_slider, SIGNAL(valueChanged(int)), &m_settings, SLOT(setRotationSensitivity(int)));
    connect(m_rotation_sensitivity_slider, SIGNAL(valueChanged(int)), this, SLOT(emit_rotation_sensitivity_changed()));

    connect(m_translation_sensitivity_slider, SIGNAL(valueChanged(int)), &m_settings, SLOT(setTranslationSensitivity(int)));
    connect(m_translation_sensitivity_slider, SIGNAL(valueChanged(int)), this, SLOT(emit_translation_sensitivity_changed()));
}

float SettingsDialog::getRotationSensitivity()
{
    return ((float) m_rotation_sensitivity_slider->value())/m_rotation_sensitivity_slider->maximum();
}

float SettingsDialog::getTranslationSensitivity()
{
    return ( (float) m_translation_sensitivity_slider->value())/m_translation_sensitivity_slider->maximum();
}

void SettingsDialog::emit_translation_sensitivity_changed()
{
    emit translationSensitivityChanged(getTranslationSensitivity());
}

void SettingsDialog::emit_rotation_sensitivity_changed()
{
    emit rotationSensitivityChanged(getRotationSensitivity());
}

QString SettingsDialog::get_two_number_digit(int digit)
{
    return QString("%1").arg(digit, 2, 10, QChar('0'));
}

QSize SettingsDialog::sizeHint() const
{
    return QSize(700,350);
}
