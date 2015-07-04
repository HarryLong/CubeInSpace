#include "settings.h"
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


// Defaults
const int Settings::_default_scale (30);
const bool Settings::_default_use_terrain_default_scale (true);
// Keys
const QString Settings::_key_rotation_sensitivity ("rotation_sensitivity");
const QString Settings::_key_translation_sensitivity ("translation_sensitivity");
const QString Settings::_key_terrain_scale ("meters_per_terrain_unit");
const QString Settings::_key_use_terrain_default_scale ("use_terrain_default_meters_per_terrain_unit");
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

void Settings::setTerrainScale(int scale)
{
    m_core_settings.setValue(Settings::_key_terrain_scale, scale);
    m_core_settings.sync();
}

void Settings::setUseTerrainDefaultScale(bool use_default)
{
    m_core_settings.setValue(Settings::_key_use_terrain_default_scale, use_default);
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

int Settings::getTerrainScale() const
{
    return m_core_settings.value(Settings::_key_terrain_scale, _default_scale).toInt();
}

bool Settings::useTerrainDefaultScale() const
{
    return m_core_settings.value(Settings::_key_use_terrain_default_scale, _default_use_terrain_default_scale).toBool();
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
    m_terrain_scale_le = new QIntLineEdit(1, 100, this);
    m_terrain_scale_le->setText(QString::number(m_settings.getTerrainScale()));

    m_default_scale_cb = new QCheckBox(this);
    m_default_scale_cb->setChecked(m_settings.useTerrainDefaultScale());

    QVBoxLayout * main_layout = new QVBoxLayout;
    // Camera sensitivity
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->addWidget(new QLabel("Rotation sensitivity: "));
        layout->addWidget(m_rotation_sensitivity_slider,1);
        main_layout->addLayout(layout);
    }

    // Z sensitivity
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->addWidget(new QLabel("Translation sensitivity "));
        layout->addWidget(m_translation_sensitivity_slider,1);
        main_layout->addLayout(layout);
    }

    // Meters per terrain unit
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->addWidget(new QLabel("Terrain scale: "));
        layout->addWidget(m_terrain_scale_le, 1);
        layout->addWidget(m_default_scale_cb);
        layout->addWidget(new QLabel("Use terrain default"));
        main_layout->addLayout(layout);
    }

    main_layout->addWidget(new QLabel(""), 1); // Padding

    setLayout(main_layout);
}

void SettingsDialog::init_signals()
{
    connect(m_rotation_sensitivity_slider, SIGNAL(valueChanged(int)), &m_settings, SLOT(setRotationSensitivity(int)));
    connect(m_translation_sensitivity_slider, SIGNAL(valueChanged(int)), &m_settings, SLOT(setTranslationSensitivity(int)));
    connect(m_terrain_scale_le, SIGNAL(valueChanged(int)), &m_settings, SLOT(setTerrainScale(int)));
    connect(m_default_scale_cb, SIGNAL(clicked(bool)), &m_settings, SLOT(setUseTerrainDefaultScale(bool)));

    connect(m_default_scale_cb, SIGNAL(clicked(bool)), m_terrain_scale_le, SLOT(setDisabled(bool)));
}

QString SettingsDialog::get_two_number_digit(int digit)
{
    return QString("%1").arg(digit, 2, 10, QChar('0'));
}

QSize SettingsDialog::sizeHint() const
{
    return QSize(700,350);
}
