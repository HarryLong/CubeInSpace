#include "central_widget.h"
#include <QBoxLayout>
#include "gl_core/glwidget.h"

CentralWidget::CentralWidget(AllActions * actions, QWidget * parent) : QWidget(parent)
{
    m_glwidget = new GLWidget(actions, this);

    connect(m_glwidget, SIGNAL(resourceModeTriggered()), this, SLOT(emit_enable_menus()));
    connect(m_glwidget, SIGNAL(clusteringModeTriggered()), this, SLOT(emit_disable_menus()));
    connect(m_glwidget, SIGNAL(plantModeTriggered()), this, SLOT(emit_disable_menus()));

    QBoxLayout * layout(new QVBoxLayout);
    layout->setMargin(0);
    layout->addWidget(m_glwidget,1);
    setLayout(layout);
}

CentralWidget::~CentralWidget()
{

}


void CentralWidget::emit_disable_menus()
{
    emit disable_menus();
}

void CentralWidget::emit_enable_menus()
{
    emit enable_menus();
}
