#include "water_shortcuts_controller.h"
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QBoxLayout>

//---------------------------------------------------------

AbsoluteHeightShortcutRow::AbsoluteHeightShortcutRow(QWidget * parent) :
    ShortcutRow("Height (m.): ", 0, 10000, parent)
{

}

AbsoluteHeightShortcutRow::~AbsoluteHeightShortcutRow()
{

}

//---------------------------------------------------------

WaterShortcutWidget::WaterShortcutWidget(int alignment, QWidget * parent) :
    QWidget(parent),
    m_alignment(alignment),
    m_absolute_height_shortcut(new AbsoluteHeightShortcutRow(this))
{
    setFixedWidth(250);
    connect(m_absolute_height_shortcut->m_ok_btn, SIGNAL(clicked(bool)), this, SLOT(emit_absolute_height()));

    init_layout();
}

WaterShortcutWidget::~WaterShortcutWidget()
{

}

int WaterShortcutWidget::alignment()
{
    return m_alignment;
}

void WaterShortcutWidget::init_layout()
{
    QVBoxLayout * layout = new QVBoxLayout;

    layout->addWidget(m_absolute_height_shortcut,0);
    layout->addWidget(new QLabel(this),1); // PADDING

    setLayout(layout);
}

int WaterShortcutWidget::getAbsoluteHeight()
{
    return m_absolute_height_shortcut->value();
}

void WaterShortcutWidget::emit_absolute_height()
{
    emit absoluteHeight(getAbsoluteHeight());
}
