#include "mode_controller.h"
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>

ModeControllerWidget::ModeControllerWidget(int alignment, QString label, QWidget * parent, Qt::WindowFlags f) : QWidget(parent, f), m_alignment(alignment)
{
    init_button();
    init_layout();
    setLabel(label);

    connect(m_btn, SIGNAL(clicked(bool)), this, SLOT(emit_triggered()));
}

ModeControllerWidget::~ModeControllerWidget()
{

}

int ModeControllerWidget::alignment()
{
    return m_alignment;
}

void ModeControllerWidget::init_button()
{
    m_btn = new QPushButton("", this);
}

void ModeControllerWidget::setLabel(QString lbl)
{
    m_btn->setText(lbl);
}

void ModeControllerWidget::emit_triggered()
{
    emit triggered();
}

void ModeControllerWidget::init_layout()
{
    QVBoxLayout * layout = new QVBoxLayout;

    layout->addWidget(m_btn, 1, Qt::AlignCenter);

    setLayout(layout);
}
