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
#include <QRadioButton>
#include <QButtonGroup>

const int SensitivitySlider::_DEFAULT = 50;
SensitivitySlider::SensitivitySlider(int value, QWidget * parent) : QSlider(Qt::Orientation::Horizontal, parent)
{
    setRange(0,100);
    setValue(value);
    push();
}

SensitivitySlider::~SensitivitySlider()
{

}

void SensitivitySlider::pop()
{
    setValue(m_pushed_value);
}

void SensitivitySlider::push()
{
    m_pushed_value = value();
}

//---------------------------------------------------------------------------

ControlModeSelectionWidget::ControlModeSelectionWidget(Mode mode, QWidget * parent) : QWidget(parent),
    m_fps_btn(new QRadioButton(this)), m_softimage_btn(new QRadioButton(this))
{
    init_layout();
    setMode(mode);
    push();
}

ControlModeSelectionWidget::~ControlModeSelectionWidget()
{

}

void ControlModeSelectionWidget::pop()
{
    setMode(m_cached_mode);
}

void ControlModeSelectionWidget::push()
{
    m_cached_mode = getActiveControlMode();
}

void ControlModeSelectionWidget::setMode(Mode mode)
{
    if(mode == Mode::_FPS)
        m_fps_btn->setChecked(true);
    else
        m_softimage_btn->setChecked(true);
}

ControlModeSelectionWidget::Mode ControlModeSelectionWidget::getActiveControlMode()
{
    return  m_fps_btn->isChecked() ? Mode::_FPS : Mode::_SOFTIMAGE;
}

void ControlModeSelectionWidget::init_layout()
{
    // Initialise the buttons
    QButtonGroup group(this);
    group.addButton(m_fps_btn);
    group.addButton(m_softimage_btn);

    QHBoxLayout * main_layout (new QHBoxLayout);

    main_layout->addWidget(new QLabel("Control Mode: "));
    {
        QHBoxLayout * h_layout = new QHBoxLayout;
        h_layout->addWidget(new QLabel("FPS"));
        h_layout->addWidget(m_fps_btn);
        main_layout->addLayout(h_layout);
    }
    {
        QHBoxLayout * h_layout = new QHBoxLayout;
        h_layout->addWidget(new QLabel("SoftImage"));
        h_layout->addWidget(m_softimage_btn);
        main_layout->addLayout(h_layout);
    }

    setLayout(main_layout);
}

//---------------------------------------------------------------------------

// Keys
const QString Settings::_key_rotation_sensitivity ("rotation_sensitivity");
const QString Settings::_key_translation_sensitivity ("translation_sensitivity");
const QString Settings::_key_control_mode ("control_mode");
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

ControlModeSelectionWidget::Mode Settings::getControlMode()
{
    int value(m_core_settings.value(Settings::_key_control_mode, ControlModeSelectionWidget::_DEFAULT).toInt());
    return static_cast<ControlModeSelectionWidget::Mode>(value);
}

void Settings::setControlMode(ControlModeSelectionWidget::Mode mode)
{
    m_core_settings.setValue(Settings::_key_control_mode, static_cast<int>(mode));
    m_core_settings.sync();
}


/*******************
 * SETTINGS EDITOR *
 *******************/
SettingsDialog::SettingsDialog ( QWidget * parent, Qt::WindowFlags f) : QDialog(parent,f),
    m_ok_btn(new QPushButton("OK", parent)),
    m_cancel_btn(new QPushButton("Cancel", parent))
{
    m_rotation_sensitivity_slider = new SensitivitySlider(m_settings.getRotationSensitivity(), parent);
    m_translation_sensitivity_slider = new SensitivitySlider(m_settings.getTranslationSensitivity(), parent);
    m_control_mode_selection_widget = new ControlModeSelectionWidget(m_settings.getControlMode(), parent);

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

    // Control Mode
    {
        main_layout->addWidget(m_control_mode_selection_widget);
    }

    main_layout->addWidget(new QLabel(""), 1); // Padding

    // OK & CANNCEL BUTTON
    {
        QHBoxLayout * layout = new QHBoxLayout;
        layout->addWidget(m_cancel_btn,0, Qt::AlignCenter);
        layout->addWidget(m_ok_btn,0, Qt::AlignCenter);

        main_layout->addLayout(layout);
    }

    setLayout(main_layout);
}

void SettingsDialog::save()
{
    m_settings.setControlMode(m_control_mode_selection_widget->getActiveControlMode());
    m_settings.setTranslationSensitivity(m_translation_sensitivity_slider->value());
    m_settings.setRotationSensitivity(m_rotation_sensitivity_slider->value());
}

void SettingsDialog::dismiss()
{
    m_control_mode_selection_widget->pop();
    m_translation_sensitivity_slider->pop();
    m_rotation_sensitivity_slider->pop();
}

void SettingsDialog::init_signals()
{
    connect(m_ok_btn, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(m_cancel_btn, SIGNAL(clicked(bool)), this, SLOT(reject()));

    connect(this, SIGNAL(accepted()), this, SLOT(save()));
    connect(this, SIGNAL(rejected()), this, SLOT(dismiss()));
}

float SettingsDialog::getRotationSensitivity()
{
    return ((float) m_rotation_sensitivity_slider->value())/m_rotation_sensitivity_slider->maximum();
}

float SettingsDialog::getTranslationSensitivity()
{
    return ( (float) m_translation_sensitivity_slider->value())/m_translation_sensitivity_slider->maximum();
}

ControlModeSelectionWidget::Mode SettingsDialog::getControlMode()
{
    return m_control_mode_selection_widget->getActiveControlMode();
}

bool SettingsDialog::fps()
{
   return getControlMode() == ControlModeSelectionWidget::Mode::_FPS;
}

bool SettingsDialog::softimage()
{
    return getControlMode() == ControlModeSelectionWidget::Mode::_SOFTIMAGE;
}

QString SettingsDialog::get_two_number_digit(int digit)
{
    return QString("%1").arg(digit, 2, 10, QChar('0'));
}

QSize SettingsDialog::sizeHint() const
{
    return QSize(700,350);
}
