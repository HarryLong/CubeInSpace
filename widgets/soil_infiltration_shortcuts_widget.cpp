#include "soil_infiltration_shortcuts_widget.h"
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QBoxLayout>
#include "soil_infiltration_controller.h"

ShortcutRow::ShortcutRow(QString label, int min, int max, QWidget * parent) :
    QWidget(parent),
    m_label(new QLabel(label, this)),
    m_input(new QIntLineEdit(min, max, min, this)),
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

    layout->addWidget(m_label,1, Qt::AlignRight);
    layout->addWidget(m_input,1);//,0, Qt::AlignCenter);
    layout->addWidget(m_ok_btn,1);//,0, Qt::AlignRight);

    setLayout(layout);
}

int ShortcutRow::value()
{
    return m_input->value();
}

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
