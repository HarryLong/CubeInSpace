#include "latitude_controller_dialog.h"
#include "constants.h"

#include <QBoxLayout>
#include <QLabel>\

#include <iostream>

LatitudeSlider::LatitudeSlider()
{
    setOrientation(Qt::Horizontal);
    setRange(-90,90);
    setValue(DEFAULT_LATITUDE);
}

LatitudeControllerDialog::LatitudeControllerDialog(QWidget * parent, Qt::WindowFlags f) :
    m_latitude_slider(new LatitudeSlider), m_latitude_lbl(new QLabel)
{
    setModal(false);
    connect(m_latitude_slider, SIGNAL(valueChanged(int)), this, SLOT(refresh_labels()));

    init_layout();
}

LatitudeControllerDialog::~LatitudeControllerDialog()
{

}

QSize LatitudeControllerDialog::sizeHint() const
{
    return QSize(500, 200);
}

void LatitudeControllerDialog::init_layout()
{
    refresh_labels(); // Add content to labels

    QVBoxLayout * main_layout = new QVBoxLayout;

    // Latittude
    {
        QHBoxLayout * layout (new QHBoxLayout);
        // Label
        layout->addWidget(new QLabel("Latitude: "),0);
        // Slider
        layout->addWidget(m_latitude_slider,1);
        // Value label
        layout->addWidget(m_latitude_lbl, 0);

        main_layout->addLayout(layout,0);
    }

    // Padding
    main_layout->addWidget(new QLabel(""),1);

    setLayout(main_layout);
}

int LatitudeControllerDialog::getLatitude()
{
    return m_latitude_slider->value();
}

void LatitudeControllerDialog::refresh_labels()
{
    m_latitude_lbl->setText(format_latitude(getLatitude()));
}

QString LatitudeControllerDialog::format_latitude(int latitude)
{
    bool positif(latitude >= 0);
    QString two_digit_latitude(get_two_number_digit(std::abs(latitude)));
    return ((positif ? "+" : "-") + two_digit_latitude + "°");
}

QString LatitudeControllerDialog::get_two_number_digit(int digit)
{
    return QString("%1").arg(digit, 2, 10, QChar('0'));
}

