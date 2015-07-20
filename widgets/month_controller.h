#ifndef MONTH_CONTROLLER_H
#define MONTH_CONTROLLER_H

#include "base_slider_controller.h"
#include <map>
#include <QString>

/***************************
 * MONTH CONTROLLER WIDGET *
 ***************************/
class MonthSlider : public QSlider
{
public:
    MonthSlider(QWidget * parent = 0);
    ~MonthSlider();
};
class MonthControllerWidget : public BaseSliderControllerWidget
{
    Q_OBJECT
public:
    MonthControllerWidget(int alignment, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~MonthControllerWidget();
    int getMonth() const;

    static std::map<int, QString> create_map();
protected slots:
    virtual void refresh_label(int value);

private:
    static const std::map<int,QString> m_num_to_month;
};


#endif // MONTH_CONTROLLER_H
