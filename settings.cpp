#include "settings.h"
#include <QTextStream>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <iostream>
#include <QLabel>
#include <QCoreApplication>
#include <QSlider>
#include <QPushButton>
#include <QCheckBox>
#include "custom_line_edit.h"

// Defaults
const int Settings::_default_camera_sensitivity (1);
const int Settings::_default_z_sensitivity (5) ;
const int Settings::_default_x_y_sensitivity (5);
const int Settings::_default_scale (30);
const bool Settings::_default_use_terrain_default_scale (true);
// Keys
const QString Settings::_key_camera_sensitivity ("camera_sensitivity");
const QString Settings::_key_z_sensitivity ("z_sensitivity");
const QString Settings::_key_x_y_sensitivity ("x_y_sensitivity");
const QString Settings::_key_terrain_scale ("meters_per_terrain_unit");
const QString Settings::_key_use_terrain_default_scale ("use_terrain_default_meters_per_terrain_unit");
Settings::Settings() : m_core_settings(QCoreApplication::organizationName(), QCoreApplication::applicationName())
{

}

Settings::~Settings()
{

}

void Settings::setCameraSensitivity(int sensitivity)
{
    m_core_settings.setValue(Settings::_key_camera_sensitivity, sensitivity);
    m_core_settings.sync();
}

void Settings::setZSensitivity(int sensitivity)
{
    m_core_settings.setValue(Settings::_key_z_sensitivity, sensitivity);
    m_core_settings.sync();
}

void Settings::setXYSensitivity(int sensitivity)
{
    m_core_settings.setValue(Settings::_key_x_y_sensitivity, sensitivity);
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

int Settings::getCameraSensitivity() const
{
    return m_core_settings.value(Settings::_key_camera_sensitivity, _default_camera_sensitivity).toInt();
}

int Settings::getZSensitivity() const
{
    return m_core_settings.value(Settings::_key_z_sensitivity, _default_z_sensitivity).toInt();
}

int Settings::getXYSensitivity() const
{
    return m_core_settings.value(Settings::_key_x_y_sensitivity, _default_x_y_sensitivity).toInt();
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
SettingsDialog::SettingsDialog ( QWidget * parent, Qt::WindowFlags f) : QDialog(parent,f)
{
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
    m_camera_sensitivity_slider = new QSlider(Qt::Horizontal, this);
    m_camera_sensitivity_slider->setRange(1,50);
    m_camera_sensitivity_slider->setValue(m_settings.getCameraSensitivity());

    m_z_movement_sensitivity_slider = new QSlider(Qt::Horizontal, this);
    m_z_movement_sensitivity_slider->setRange(1,100);
    m_z_movement_sensitivity_slider->setValue(m_settings.getZSensitivity());

    m_x_y_movement_sensitivity_slider = new QSlider(Qt::Horizontal, this);
    m_x_y_movement_sensitivity_slider->setRange(1,50);
    m_x_y_movement_sensitivity_slider->setValue(m_settings.getXYSensitivity());

    m_terrain_scale_le = new QIntLineEdit(1, 100, this);
    m_terrain_scale_le->setText(QString::number(m_settings.getTerrainScale()));

    m_default_scale_cb = new QCheckBox(this);
    m_default_scale_cb->setChecked(m_settings.useTerrainDefaultScale());

    QVBoxLayout * main_layout = new QVBoxLayout;
    // Camera sensitivity
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->addWidget(new QLabel("Camera sensitivity: "));
        layout->addWidget(m_camera_sensitivity_slider,1);
        main_layout->addLayout(layout);
    }

    // Z sensitivity
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->addWidget(new QLabel("ZX plane (forward/back) movement sensitivity: "));
        layout->addWidget(m_z_movement_sensitivity_slider,1);
        main_layout->addLayout(layout);
    }

    // X-Y sensitivity
    {
        QHBoxLayout * layout = new QHBoxLayout();
        layout->addWidget(new QLabel("XY plane (side/up) movement sensitivity: "));
        layout->addWidget(m_x_y_movement_sensitivity_slider,1);
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
    connect(m_camera_sensitivity_slider, SIGNAL(valueChanged(int)), &m_settings, SLOT(setCameraSensitivity(int)));
    connect(m_z_movement_sensitivity_slider, SIGNAL(valueChanged(int)), &m_settings, SLOT(setZSensitivity(int)));
    connect(m_x_y_movement_sensitivity_slider, SIGNAL(valueChanged(int)), &m_settings, SLOT(setXYSensitivity(int)));
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
