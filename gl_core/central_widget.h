#ifndef CENTRAL_WIDGET_H
#define CENTRAL_WIDGET_H

#include <QWidget>
class GLWidget;
class AllActions;
class CentralWidget : public QWidget
{
    Q_OBJECT
public:
    CentralWidget(AllActions * actions, QWidget * parent = nullptr);
    ~CentralWidget();    

signals:
    void disable_menus();
    void enable_menus();

private slots:
    void emit_disable_menus();
    void emit_enable_menus();

private:
    GLWidget * m_glwidget;
};

#endif // CENTRAL_WIDGET_H
