#include "central_widget.h"
#include <QBoxLayout>
#include "gl_core/glwidget.h"

CentralWidget::CentralWidget(AllActions * actions, QWidget * parent) : QWidget(parent)
{
    m_glwidget = new GLWidget(actions, this);


    QBoxLayout * layout(new QVBoxLayout);
    layout->setMargin(0);
    layout->addWidget(m_glwidget,1);
    setLayout(layout);
}

CentralWidget::~CentralWidget()
{

}
