#ifndef BASE_SLIDER_CONTROLLER_H
#define BASE_SLIDER_CONTROLLER_H

#include <QSlider>
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

class BaseSliderControllerWidget : public QWidget
{
Q_OBJECT
public:
    BaseSliderControllerWidget(QSlider * slider, QString heading, int alignment, QWidget * parent = 0, Qt::WindowFlags f = 0);
    virtual ~BaseSliderControllerWidget();
    int value() const;
    QSlider * getSlider();
//    void resize();
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

#endif // BASE_SLIDER_CONTROLLER_H
