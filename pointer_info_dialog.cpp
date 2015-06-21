#include "pointer_info_dialog.h"
#include <QLabel>
#include <QBoxLayout>

PointerInformationDialog::PointerInformationDialog(QWidget * parent, Qt::WindowFlags f) : QDialog(parent, f),
    m_altitude_lbl(new QLabel), m_slope_lbl(new QLabel), m_min_temp_lbl(new QLabel), m_max_temp_lbl(new QLabel),
    m_shade_lbl(new QLabel), m_daily_min_illumination_lbl(new QLabel), m_daily_max_illumination_lbl(new QLabel)
{
    setWindowTitle("Pointer Information");
    setModal(false);
    init_layout();
}

PointerInformationDialog::~PointerInformationDialog()
{

}

QSize PointerInformationDialog::minimumSizeHint() const
{
    return QSize(200,400);
}

void PointerInformationDialog::setAltitude(float altitude)
{
    m_altitude_lbl->setText(QString::number(altitude));
}

void PointerInformationDialog::setSlope(float slope)
{
    m_slope_lbl->setText(QString::number(slope));
}

void PointerInformationDialog::setMinTemp(float min_temp)
{
    m_min_temp_lbl->setText(QString::number(min_temp));
}

void PointerInformationDialog::setMaxTemp(float max_temp)
{
    m_max_temp_lbl->setText(QString::number(max_temp));
}

void PointerInformationDialog::setShaded(bool shaded)
{
    m_shade_lbl->setText(shaded ? "Y" : "N");
}

void PointerInformationDialog::setMinDailyIllumination(int min_daily_illumination)
{
    m_daily_min_illumination_lbl->setText(QString::number(min_daily_illumination));
}

void PointerInformationDialog::setMaxDailyIllumination(int max_daily_illumination)
{
    m_daily_max_illumination_lbl->setText(QString::number(max_daily_illumination));
}

void PointerInformationDialog::invalidPoint()
{
    setInvalid(m_altitude_lbl);
    setInvalid(m_slope_lbl);
    setInvalid(m_min_temp_lbl);
    setInvalid(m_max_temp_lbl);
    setInvalid(m_shade_lbl);
    setInvalid(m_daily_min_illumination_lbl);
    setInvalid(m_daily_max_illumination_lbl);
}

void PointerInformationDialog::update(float altitude, float slope,
            bool shade_set, bool shaded,
            bool temp_set, float min_temp, float max_temp,
            bool daily_illumination_set, int min_daily_illumination, int max_daily_illumination)
{
    setAltitude(altitude);
    setSlope(slope);

    if(shade_set)
        setShaded(shaded);
    else
        setInvalid(m_shade_lbl);

    if(temp_set)
    {
        setMinTemp(min_temp);
        setMaxTemp(max_temp);
    }
    else
    {
        setInvalid(m_min_temp_lbl);
        setInvalid(m_max_temp_lbl);
    }

    if(daily_illumination_set)
    {
        setMinDailyIllumination(min_daily_illumination);
        setMaxDailyIllumination(max_daily_illumination);
    }
    else
    {
        setInvalid(m_daily_min_illumination_lbl);
        setInvalid(m_daily_max_illumination_lbl);
    }
}

void PointerInformationDialog::setInvalid(QLabel * lbl)
{
    lbl->setText(" - ");
}

void PointerInformationDialog::init_layout()
{
    QVBoxLayout * main_layout (new QVBoxLayout);

    // Altitude
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Altitude (m.): "), 1, Qt::AlignLeft);
        layout->addWidget(m_altitude_lbl, 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Slope
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Slope (°): "), 1, Qt::AlignLeft);
        layout->addWidget(m_slope_lbl, 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Min temp
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Minimum temp: "), 1, Qt::AlignLeft);
        layout->addWidget(m_min_temp_lbl, 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Max temp
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Maximum temp: "), 1, Qt::AlignLeft);
        layout->addWidget(m_max_temp_lbl, 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Shade
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Shaded: "), 1, Qt::AlignLeft);
        layout->addWidget(m_shade_lbl, 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Min daily illumination
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Min daily illumination (h.): "), 1, Qt::AlignLeft);
        layout->addWidget(m_daily_min_illumination_lbl, 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Max daily illumination
    {
        QHBoxLayout * layout (new QHBoxLayout);


        layout->addWidget(new QLabel("Max daily illumination (h.): "), 1, Qt::AlignLeft);
        layout->addWidget(m_daily_max_illumination_lbl, 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    main_layout->addWidget(new QLabel(""), 1); // Padding

    setLayout(main_layout);
}
