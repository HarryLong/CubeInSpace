#include "pointer_info_dlg.h"
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
    m_labels[LabelType::_POINT] = new QLabel(this);
    m_labels[LabelType::_ALTITUDE] = new QLabel(this);
    m_labels[LabelType::_SLOPE] = new QLabel(this);
    m_labels[LabelType::_SOIL_INFILTRATION_RATE] = new QLabel(this);
    m_labels[LabelType::_SOIL_HUMIDITY] = new QLabel(this);
    m_labels[LabelType::_WEIGHTED_SOIL_HUMIDITY] = new QLabel(this);
    m_labels[LabelType::_SHADE] = new QLabel(this);
    m_labels[LabelType::_TEMP] = new QLabel(this);
    m_labels[LabelType::_MIN_DAILY_ILLUMINATION] = new QLabel(this);
    m_labels[LabelType::_MAX_DAILY_ILLUMINATION] = new QLabel(this);
    m_labels[LabelType::_WATER_HEIGHT] = new QLabel(this);
    m_labels[LabelType::_AGGREGATE_HEIGHT] = new QLabel(this);
}

QSize PointerInformationDialog::minimumSizeHint() const
{
    return QSize(200,400);
}


void PointerInformationDialog::setPoint(const glm::vec2 & point)
{
    QString point_str("[");
    point_str.append(QString::number(point.x)).append(",").append(QString::number(point.y)).append("]");
    m_labels[LabelType::_POINT]->setText(point_str);
}

void PointerInformationDialog::setAltitude(float altitude)
{
    m_labels[LabelType::_ALTITUDE]->setText(QString::number(altitude));
}

void PointerInformationDialog::setSlope(float slope)
{
    m_labels[LabelType::_SLOPE]->setText(QString::number(slope));
}

void PointerInformationDialog::setSoilInfiltrationRate(int soil_infiltration_rate)
{
    m_labels[LabelType::_SOIL_INFILTRATION_RATE]->setText(QString::number(soil_infiltration_rate));
}

void PointerInformationDialog::setShaded(bool shaded)
{
    m_labels[LabelType::_SHADE]->setText(shaded ? "Y" : "N");
}

void PointerInformationDialog::setTemp(float temp)
{
    m_labels[LabelType::_TEMP]->setText(QString::number(temp));
}

void PointerInformationDialog::setMinDailyIllumination(int min_daily_illumination)
{
    m_labels[LabelType::_MIN_DAILY_ILLUMINATION]->setText(QString::number(min_daily_illumination));
}

void PointerInformationDialog::setMaxDailyIllumination(int max_daily_illumination)
{
    m_labels[LabelType::_MAX_DAILY_ILLUMINATION]->setText(QString::number(max_daily_illumination));
}

void PointerInformationDialog::setWaterHeight(float water_height)
{
    m_labels[LabelType::_WATER_HEIGHT]->setText(QString::number(water_height));
}

void PointerInformationDialog::setSoilHumidity(int soil_humidity)
{
    m_labels[LabelType::_SOIL_HUMIDITY]->setText(QString::number(soil_humidity));
}

void PointerInformationDialog::setWeightedSoilHumidity(float weighted_soil_humidity)
{
    m_labels[LabelType::_WEIGHTED_SOIL_HUMIDITY]->setText(QString::number(weighted_soil_humidity));
}

void PointerInformationDialog::setAggregateHeight(int height)
{
    m_labels[LabelType::_AGGREGATE_HEIGHT]->setText(QString::number(height));
}

void PointerInformationDialog::invalidPoint()
{
    for(std::pair<LabelType, QLabel*> lbl_pair : m_labels)
        setInvalid(lbl_pair.second);
}

void PointerInformationDialog::update(const glm::vec2 & point,
                                      float altitude, float slope, float water_height, int soil_infiltration_rate, int soil_humidity, float weighted_soil_humidity,
                                      bool shade_set, bool shaded,
                                      bool temp_set, float temp,
                                      bool daily_illumination_set, int min_daily_illumination, int max_daily_illumination)
{
    //POINT
    setPoint(point);
    // ALTITUDE
    setAltitude(altitude);
    // SLOPE
    setSlope(slope);
    // WATER HEIGHT
    setWaterHeight(water_height);

    // SOIL INFILTRATION RATE
    setSoilInfiltrationRate(soil_infiltration_rate);

    // SOIL HUMIDITY
    setSoilHumidity(soil_humidity);

    // WEIGHTED SOIL HUMIDITY
    setWeightedSoilHumidity(weighted_soil_humidity);

    // Aggregate Height
    {
        setAggregateHeight(altitude*1000 + water_height);
    }

    // SHADE
    if(shade_set)
        setShaded(shaded);
    else
        setInvalid(m_labels[LabelType::_SHADE]);

    // MIN-MAX TEMPERATURE
    if(temp_set)
    {
        setTemp(temp);
    }
    else
    {
        setInvalid(m_labels[LabelType::_TEMP]);
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
    // Point
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Point: "), 1, Qt::AlignLeft);
        layout->addWidget(m_labels[LabelType::_POINT], 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }
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

    // Soil Infiltration Rate
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Soil infiltration rate (mm/h): "), 1, Qt::AlignLeft);
        layout->addWidget(m_labels[LabelType::_SOIL_INFILTRATION_RATE], 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Soil Humidity
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Soil humidity (mm): "), 1, Qt::AlignLeft);
        layout->addWidget(m_labels[LabelType::_SOIL_HUMIDITY], 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Weighted Soil Humidity
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Weighted soil humidity (mm): "), 1, Qt::AlignLeft);
        layout->addWidget(m_labels[LabelType::_WEIGHTED_SOIL_HUMIDITY], 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    // Min temp
    {
        QHBoxLayout * layout (new QHBoxLayout);

        layout->addWidget(new QLabel("Temperature: "), 1, Qt::AlignLeft);
        layout->addWidget(m_labels[LabelType::_TEMP], 1, Qt::AlignRight);

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


    // Aggregate height
    {
        QHBoxLayout * layout (new QHBoxLayout);


        layout->addWidget(new QLabel("Aggregate height (mm): "), 1, Qt::AlignLeft);
        layout->addWidget(m_labels[LabelType::_AGGREGATE_HEIGHT], 1, Qt::AlignRight);

        main_layout->addLayout(layout, 0);
    }

    main_layout->addWidget(new QLabel(""), 1); // Padding

    setLayout(main_layout);
}
