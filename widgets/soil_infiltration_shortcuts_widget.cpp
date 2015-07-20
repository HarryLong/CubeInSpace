#include "soil_infiltration_shortcuts_widget.h"
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QBoxLayout>

ShortcutRow::ShortcutRow(QString label, QWidget * parent) :
    QWidget(parent),
    m_label(new QLabel(label, this)),
    m_input(new QIntLineEdit(0, 90, 90, this)),
    m_ok_btn(new QPushButton("OK", this))
{
    QPalette palette;
    palette.setColor(QPalette::Foreground,Qt::white);
    palette.setColor(QPalette::Text,Qt::white);

    m_label->setPalette(palette);
    m_input->setPalette(palette);
    setFixedWidth(250);
    init_layout();
}

ShortcutRow::~ShortcutRow()
{

}

void ShortcutRow::init_layout()
{
    QHBoxLayout * layout = new QHBoxLayout;

    layout->addWidget(m_label);//,0, Qt::AlignLeft);
    layout->addWidget(m_input);//,0, Qt::AlignCenter);
    layout->addWidget(m_ok_btn);//,0, Qt::AlignRight);

    setLayout(layout);
}

int ShortcutRow::value()
{
    return m_input->value();
}

//---------------------------------------------------------

SoilInfiltrationShortcutWidget::SoilInfiltrationShortcutWidget(int alignment, QWidget * parent) :
    QWidget(parent),
    m_alignment(alignment),
    m_slope_dependant_shortcut(new ShortcutRow("Slope >=: ", this))
{
    setFixedWidth(250);
    connect(m_slope_dependant_shortcut->m_ok_btn, SIGNAL(clicked(bool)), this, SLOT(emit_soil_infiltration_zero_over_slope()));
}

SoilInfiltrationShortcutWidget::~SoilInfiltrationShortcutWidget()
{

}

int SoilInfiltrationShortcutWidget::alignment()
{
    return m_alignment;
}

void SoilInfiltrationShortcutWidget::init_layout()
{
    QVBoxLayout * layout = new QVBoxLayout;

    layout->addWidget(m_slope_dependant_shortcut);

    setLayout(layout);
}

int SoilInfiltrationShortcutWidget::getSoilInfiltrationZeroOverSlope()
{
    return m_slope_dependant_shortcut->value();
}

void SoilInfiltrationShortcutWidget::emit_soil_infiltration_zero_over_slope()
{
    emit soilInfiltrationZeroOverSlope(getSoilInfiltrationZeroOverSlope());
}
