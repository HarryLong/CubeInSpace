#include "settings.h"
#include <QTextStream>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <iostream>
#include <QLabel>

/*******************
 * SETTINGS FILE*
 *******************/
#define SETTINGS_FILE_NAME ".settings"

#define CAMERA_SENSITIVITY_PREFIX "camera_sensitivity"
#define Z_MOVEMENT_SENSITIVITY_PREFIX "z_movement_sensitivity"
#define X_Y_MOVEMENT_SENSITIVITY_PREFIX "x_y_movement_sensitivity"
#define TERRAIN_DIM_PREFIX "terrain_dim"

SettingsFile::SettingsFile()
{
    QFile file(SETTINGS_FILE_NAME);
    if(file.exists())
    {
        if(!file.open(QIODevice::ReadOnly))
        {
            std::cerr << "Failed to open file: " << SETTINGS_FILE_NAME << " to read the settings " << std::endl;
            exit(1);
        }
        QTextStream in(&file);

        while(!in.atEnd()) {
            QString line = in.readLine();
            QStringList split_line = line.split("=");

            QString prefix (split_line.at(0));
            if(prefix == CAMERA_SENSITIVITY_PREFIX)
                m_settings.camera_sensitivity = split_line.at(1).toInt();
            else if(prefix == Z_MOVEMENT_SENSITIVITY_PREFIX)
                m_settings.z_movement_sensitivity = split_line.at(1).toInt();
            else if(prefix == X_Y_MOVEMENT_SENSITIVITY_PREFIX)
                m_settings.x_y_movement_sensitivity = split_line.at(1).toInt();
            else if(prefix == TERRAIN_DIM_PREFIX)
                m_settings.terrain_width = split_line.at(1).toInt();
            else
                std::cerr << "Unknown prefix: " << prefix.toStdString() << std::endl;
        }
        file.close();
    }
    // else settings will be default
}

SettingsFile::~SettingsFile()
{
    QFile file(SETTINGS_FILE_NAME);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        std::cerr << "Failed to open file: " << SETTINGS_FILE_NAME << " to write the settings " << std::endl;
        exit(1);
    }

    QTextStream out(&file);
    out << CAMERA_SENSITIVITY_PREFIX << "=" << m_settings.camera_sensitivity << "\n";
    out << Z_MOVEMENT_SENSITIVITY_PREFIX << "=" << m_settings.z_movement_sensitivity << "\n";
    out << X_Y_MOVEMENT_SENSITIVITY_PREFIX << "=" << m_settings.x_y_movement_sensitivity << "\n";
    out << TERRAIN_DIM_PREFIX << "=" << m_settings.terrain_width << "\n";
}

/*******************
 * SETTINGS EDITOR *
 *******************/
SettingsEditor::SettingsEditor ( QWidget * parent, Qt::WindowFlags f)
{
    setWindowTitle("Settings");
    init_layout();
    init_signals();
    grabKeyboard();
}

void SettingsEditor::init_layout()
{
    m_camera_sensitivity_slider = new QSlider(Qt::Horizontal, this);
    m_camera_sensitivity_slider->setRange(1,50);
    m_camera_sensitivity_slider->setTickInterval(5);
    m_camera_sensitivity_slider->setValue(m_settings_file.m_settings.camera_sensitivity);

    m_z_movement_sensitivity_slider = new QSlider(Qt::Horizontal, this);
    m_z_movement_sensitivity_slider->setRange(1,100);
    m_z_movement_sensitivity_slider->setTickInterval(10);
    m_z_movement_sensitivity_slider->setValue(m_settings_file.m_settings.z_movement_sensitivity);

    m_x_y_movement_sensitivity_slider = new QSlider(Qt::Horizontal, this);
    m_x_y_movement_sensitivity_slider->setRange(1,50);
    m_x_y_movement_sensitivity_slider->setTickInterval(5);
    m_x_y_movement_sensitivity_slider->setValue(m_settings_file.m_settings.x_y_movement_sensitivity);

    m_terrain_dim_spinbox = new QSpinBox(this);
    m_terrain_dim_spinbox->setRange(100,10000);
    m_terrain_dim_spinbox->setSingleStep(100);
    m_terrain_dim_spinbox->setValue(m_settings_file.m_settings.terrain_width);

    QHBoxLayout * camera_sensitivity_hl = new QHBoxLayout();
    {
        camera_sensitivity_hl->addWidget(new QLabel("Camera sensitivity: "));
        camera_sensitivity_hl->addWidget(m_camera_sensitivity_slider);
    }

    QHBoxLayout * z_movement_sensitivity_hl = new QHBoxLayout();
    {
        z_movement_sensitivity_hl->addWidget(new QLabel("ZX plane (forward/back) movement sensitivity: "));
        z_movement_sensitivity_hl->addWidget(m_z_movement_sensitivity_slider);
    }

    QHBoxLayout * x_y_movement_sensitivity_hl = new QHBoxLayout();
    {
        x_y_movement_sensitivity_hl->addWidget(new QLabel("XY plane (side/up) movement sensitivity: "));
        x_y_movement_sensitivity_hl->addWidget(m_x_y_movement_sensitivity_slider);
    }

    QHBoxLayout * terrain_dim_hl = new QHBoxLayout();
    {
        terrain_dim_hl->addWidget(new QLabel("Terrain dimension: "));
        terrain_dim_hl->addWidget(m_terrain_dim_spinbox);
    }

    QVBoxLayout * main_layout = new QVBoxLayout;

    main_layout->addLayout(camera_sensitivity_hl);
    main_layout->addLayout(z_movement_sensitivity_hl);
    main_layout->addLayout(x_y_movement_sensitivity_hl);
    main_layout->addLayout(terrain_dim_hl);

    setLayout(main_layout);
}

void SettingsEditor::init_signals()
{
    connect(m_camera_sensitivity_slider, SIGNAL(valueChanged(int)), this, SLOT(settings_changed()));
    connect(m_z_movement_sensitivity_slider, SIGNAL(valueChanged(int)), this, SLOT(settings_changed()));
    connect(m_x_y_movement_sensitivity_slider, SIGNAL(valueChanged(int)), this, SLOT(settings_changed()));
    connect(m_terrain_dim_spinbox, SIGNAL(valueChanged(int)), this, SLOT(settings_changed()));
}

void SettingsEditor::settings_changed()
{
    m_settings_file.m_settings.camera_sensitivity = m_camera_sensitivity_slider->value();
    m_settings_file.m_settings.z_movement_sensitivity = m_z_movement_sensitivity_slider->value();
    m_settings_file.m_settings.x_y_movement_sensitivity = m_x_y_movement_sensitivity_slider->value();
    m_settings_file.m_settings.terrain_width = m_terrain_dim_spinbox->value();
}

SettingsEditor::~SettingsEditor()
{

}
