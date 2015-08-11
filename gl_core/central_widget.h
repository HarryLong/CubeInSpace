#ifndef CENTRAL_WIDGET_H
#define CENTRAL_WIDGET_H

#include <QWidget>
class GLWidget;
class AllActions;
class CentralWidget : public QWidget
{
public:
    CentralWidget(AllActions * actions, QWidget * parent = nullptr);
    ~CentralWidget();

private:
    GLWidget * m_glwidget;
};

#endif // CENTRAL_WIDGET_H
