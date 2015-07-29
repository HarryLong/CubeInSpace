#include "soil_infiltration_shortcuts_widget.h"
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QBoxLayout>
#include "soil_infiltration_controller.h"

//---------------------------------------------------------

SlopeDependentShortcutRow::SlopeDependentShortcutRow(QWidget * parent) :
    ShortcutRow("Slope >=: ", 0, 90, parent)
{

}

SlopeDependentShortcutRow::~SlopeDependentShortcutRow()
{

}

//---------------------------------------------------------

FillShortcutRow::FillShortcutRow(QWidget * parent) :
    ShortcutRow("Fill:", SoilInfiltrationControllerWidget::_MIN_INFILTRATION_RATE, SoilInfiltrationControllerWidget::_MAX_INFILTRATION_RATE, parent)
{

}

FillShortcutRow::~FillShortcutRow()
{

}

//---------------------------------------------------------

SoilInfiltrationShortcutWidget::SoilInfiltrationShortcutWidget(int alignment, QWidget * parent) :
    QWidget(parent),
    m_alignment(alignment),
    m_slope_dependant_shortcut(new SlopeDependentShortcutRow(this)),
    m_fill_shortcut(new FillShortcutRow(this))
{
    setFixedWidth(250);
    connect(m_slope_dependant_shortcut->m_ok_btn, SIGNAL(clicked(bool)), this, SLOT(emit_soil_infiltration_zero_over_slope()));
    connect(m_fill_shortcut->m_ok_btn, SIGNAL(clicked(bool)), this, SLOT(emit_fill()));

    init_layout();
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

    layout->addWidget(m_fill_shortcut,0);
    layout->addWidget(m_slope_dependant_shortcut,0);
    layout->addWidget(new QLabel(this),1); // PADDING

    setLayout(layout);
}

int SoilInfiltrationShortcutWidget::getSoilInfiltrationZeroOverSlope()
{
    return m_slope_dependant_shortcut->value();
}

int SoilInfiltrationShortcutWidget::get_fill_rate()
{
    return m_fill_shortcut->value();
}

void SoilInfiltrationShortcutWidget::emit_soil_infiltration_zero_over_slope()
{
    emit soilInfiltrationZeroOverSlope(getSoilInfiltrationZeroOverSlope());
}

void SoilInfiltrationShortcutWidget::emit_fill()
{
    emit fill(get_fill_rate());
}
