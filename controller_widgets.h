#ifndef CONTROLLER_WIDGETS_H
#define CONTROLLER_WIDGETS_H

#include <QWidget>
#include <QSlider>

class QLabel;
class BaseControllerWidget : public QWidget
{
Q_OBJECT
public:
    BaseControllerWidget(QSlider * slider, QWidget * parent = 0, Qt::WindowFlags f = 0);
    virtual ~BaseControllerWidget();

    int value() const;

    QSlider * getSlider();

protected slots:
    virtual void refresh_label(int value) = 0;

protected:
    virtual void init_layout();

    QLabel * m_label;
    QSlider * m_slider;
};


/******************************
 * LATITUDE CONTROLLER WIDGET *
 ******************************/
class LatitudeSlider : public QSlider
{
public:
    LatitudeSlider(QWidget * parent = 0);
    ~LatitudeSlider();
};
class LatitudeControllerWidget : public BaseControllerWidget
{
    Q_OBJECT
public:
    LatitudeControllerWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~LatitudeControllerWidget();
    int getLatitude() const;

protected slots:
    virtual void refresh_label(int value);
};

/**************************
 * TIME CONTROLLER WIDGET *
 **************************/
class TimeSlider : public QSlider
{
public:
    TimeSlider(QWidget * parent = 0);
    ~TimeSlider();
};
class TimeControllerWidget : public BaseControllerWidget
{
    Q_OBJECT
public:
    TimeControllerWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~TimeControllerWidget();
    int getTime() const;

protected slots:
    virtual void refresh_label(int value);
};

/***************************
 * MONTH CONTROLLER WIDGET *
 ***************************/
class MonthSlider : public QSlider
{
public:
    MonthSlider(QWidget * parent = 0);
    ~MonthSlider();
};
class MonthControllerWidget : public BaseControllerWidget
{
    Q_OBJECT
public:
    MonthControllerWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~MonthControllerWidget();
    int getTime() const;

protected slots:
    virtual void refresh_label(int value);
};

#endif // CONTROLLER_WIDGETS_H
