#include "pointer_info_dialog.h"
#include <QLabel>
#include <QBoxLayout>

PointerInformationDialog::PointerInformationDialog(QWidget * parent, Qt::WindowFlags f) : QDialog(parent, f)
{
    setWindowTitle("Pointer Information");
    setModal(false);
    init_labels();
    init_layout();
}

PointerInformationDialog::~PointerInformationDialog()
{

}

void PointerInformationDialog::init_labels()
{
    m_labels[LabelType::_ALTITUDE] = new QLabel(this);
    m_labels[LabelType::_SLOPE] = new QLabel(this);
    m_labels[LabelType::_SHADE] = new QLabel(this);
    m_labels[LabelType::_MIN_TEMP] = new QLabel(this);
    m_labels[LabelType::_MAX_TEMP] = new QLabel(this);
    m_labels[LabelType::_MIN_DAILY_ILLUMINATION] = new QLabel(this);
    m_labels[LabelType::_MAX_DAILY_ILLUMINATION] = new QLabel(this);
    m_labels[LabelType::_WATER_HEIGHT] = new QLabel(this);
}

QSize PointerInformationDialog::minimumSizeHint() const
{
    return QSize(200,400);
}

void PointerInformationDialog::setAltitude(float altitude)
{
    m_labels[LabelType::_ALTITUDE]->setText(QString::number(altitude));
}

void PointerInformationDialog::setSlope(float slope)
{
    m_labels[LabelType::_SLOPE]->setText(QString::number(slope));
}

void PointerInformationDialog::setShaded(bool shaded)
{
    m_labels[LabelType::_SHADE]->setText(shaded ? "Y" : "N");
}

void PointerInformationDialog::setMinTemp(float min_temp)
{
    m_labels[LabelType::_MIN_TEMP]->setText(QString::number(min_temp));
}

void PointerInformationDialog::setMaxTemp(float max_temp)
{
    m_labels[LabelType::_MAX_TEMP]->setText(QString::number(max_temp));
}

void PointerInformationDialog::setMinDailyIllumination(int min_daily_illumination)
{
    m_labels[LabelType::_MIN_DAILY_ILLUMINATION]->setText(QString::number(min_daily_illumination));
}

void PointerInformationDialog::setMaxDailyIllumination(int max_daily_illumination)
{
    m_labels[LabelType::_MAX_DAILY_ILLUMINATION]->setText(QString::number(max_daily_illumination));
}

void PointerInformationDialog::setWaterHeight(int water_height)
{
    m_labels[LabelType::_WATER_HEIGHT]->setText(QString::number(water_height));
}

void PointerInformationDialog::invalidPoint()
{
    for(std::pair<LabelType, QLabel*> lbl_pair : m_labels)
        setInvalid(lbl_pair.second);
}

void PointerInformationDialog::update(float altitude, float slope, int water_height,
            bool shade_set, bool shaded,
            bool temp_set, float min_temp, float max_temp,
            bool daily_illumination_set, int min_daily_illumination, int max_daily_illumination)
{
    // ALTITUDE
    setAltitude(altitude);
    // SLOPE
    setSlope(slope);
    // WATER HEIGHT
    setWaterHeight(water_height);

    // SHADE
    if(shade_set)
        setShaded(shaded);
    else
        setInvalid(m_labels[LabelType::_SHADE]);

    // MIN-MAX TEMPERATURE
    if(temp_set)
    {
        setMinTemp(min_temp);
        setMaxTemp(max_temp);
    }
    else
    {
        setInvalid(m_labels[LabelType::_MIN_TEMP]);
        setInvalid(m_labels[LabelType::_MAX_TEMP]);
    }

    // MIN-MAX DAILY ILLUMINATION
    if(daily_illumination_set)
    {
        setMinDailyIllumination(min_daily_illumination);
        setMaxDailyIllumination(max_daily_illumination);
    }
    else
    {
        setInvalid(m_labels[LabelType::_MIN_DAILY_ILLUMINATION]);
        setInvalid(m_labels[LabelType::_MAX_DAILY_ILLUMINATION]);
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
        layout->addWidget(m_labels[LabelType::_ALTITUDE], 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Slope
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Slope (°): "), 1, Qt::AlignLeft);
        layout->addWidget(m_labels[LabelType::_SLOPE], 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Min temp
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Minimum temp: "), 1, Qt::AlignLeft);
        layout->addWidget(m_labels[LabelType::_MIN_TEMP], 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Max temp
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Maximum temp: "), 1, Qt::AlignLeft);
        layout->addWidget(m_labels[LabelType::_MAX_TEMP], 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Shade
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Shaded: "), 1, Qt::AlignLeft);
        layout->addWidget(m_labels[LabelType::_SHADE], 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Min daily illumination
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Min daily illumination (h.): "), 1, Qt::AlignLeft);
        layout->addWidget(m_labels[LabelType::_MIN_DAILY_ILLUMINATION], 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Max daily illumination
    {
        QHBoxLayout * layout (new QHBoxLayout);


        layout->addWidget(new QLabel("Max daily illumination (h.): "), 1, Qt::AlignLeft);
        layout->addWidget(m_labels[LabelType::_MAX_DAILY_ILLUMINATION], 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Water height
    {
        QHBoxLayout * layout (new QHBoxLayout);


        layout->addWidget(new QLabel("Water height (mm): "), 1, Qt::AlignLeft);
        layout->addWidget(m_labels[LabelType::_WATER_HEIGHT], 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    main_layout->addWidget(new QLabel(""), 1); // Padding

    setLayout(main_layout);
}
