#include "shortcut_controller_row.h"
#include <QBoxLayout>

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
