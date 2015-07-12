#ifndef CONTROLLER_WIDGETS_H
#define CONTROLLER_WIDGETS_H

#include <QWidget>
#include <QSlider>
#include <QFont>
#include <QLabel>

class DisplayLabel : public QLabel
{
public:
    DisplayLabel(QWidget * parent);
};

class HeaderLabel : public DisplayLabel
{
public:
    HeaderLabel(QString text, QWidget * parent);

protected:
    void paintEvent(QPaintEvent*);

private:
    int m_alignment;
};

class BaseControllerWidget : public QWidget
{
Q_OBJECT
public:
    BaseControllerWidget(QSlider * slider, QString heading, int alignment, QWidget * parent = 0, Qt::WindowFlags f = 0);
    virtual ~BaseControllerWidget();
    int value() const;
    QSlider * getSlider();
    void resize();
    int alignment();

    static QString format_two_digit_num(int num);

protected slots:
    virtual void refresh_label(int value) = 0;

protected:
    virtual void init_layout();

    DisplayLabel * m_value_label;
    HeaderLabel * m_heading_label;
    QSlider * m_slider;
    const static QFont m_label_font;
    int m_alignment;
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
    LatitudeControllerWidget(int alignment, QWidget * parent = 0, Qt::WindowFlags f = 0);
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
    TimeControllerWidget(int alignment, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~TimeControllerWidget();
    int getTime() const;

protected slots:
    virtual void refresh_label(int value);

private:
    QString format_time(int time_in_minutes);
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
    MonthControllerWidget(int alignment, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~MonthControllerWidget();
    int getMonth() const;

    static std::map<int, QString> create_map();
protected slots:
    virtual void refresh_label(int value);

private:
    static const std::map<int,QString> m_num_to_month;
};

/******************************
 * CONTROLLER WIDGETS WRAPPER *
 ******************************/
class OverlayWidgets : public QObject{
    Q_OBJECT
public:
    OverlayWidgets(QWidget * parent);
    ~OverlayWidgets();

    void hideAll();
    void resize(int container_width, int container_height);

    int getLatitude();
    int getTime();
    int getMonth();

signals:
    void timeControllersStateChanged(bool active);
    void latitudeControllersStateChanged(bool active);
    void monthChanged(int month);
    void latitudeChanged(int latitude);
    void timeChanged(int time);

public slots:
    void trigger_time_controllers(bool show_widget);
    void trigger_latitude_controllers(bool show_widget);

private slots:
    void emit_latitude_changed(int latitude);
    void emit_time_changed(int time);
    void emit_month_changed(int month);

private:

    std::vector<BaseControllerWidget*> m_raw_widgets;
//    std::map<Type, std::vector<BaseControllerWidget*> > m_type_sorted_widgets;
    std::map<int, std::vector<BaseControllerWidget*> > m_alignment_sorted_widgets;

    BaseControllerWidget * m_time_widget;
    BaseControllerWidget * m_month_widget;
    BaseControllerWidget * m_latitude_widget;
};


#endif // CONTROLLER_WIDGETS_H
