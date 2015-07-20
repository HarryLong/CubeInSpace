#include "base_slider_controller.h"
#include <QPainter>
#include <QBoxLayout>

DisplayLabel::DisplayLabel(QWidget *parent) :  QLabel(parent)
{
    setStyleSheet("QLabel { color : rgb(120,120,120); }");
    setAlignment(Qt::AlignBottom);
}

HeaderLabel::HeaderLabel(QString text, QWidget * parent) :
    DisplayLabel(parent)
{
    setText(text);
}

void HeaderLabel::paintEvent(QPaintEvent*)
{
    setAlignment(Qt::AlignCenter);
    QPainter painter(this);
    painter.rotate(90);
    painter.drawText(QPoint(0,0), text());
}

/**************************
 * BASE CONTROLLER WIDGET *
 **************************/
const QFont BaseSliderControllerWidget::m_label_font("Times", 25, QFont::Weight::Bold);
BaseSliderControllerWidget::BaseSliderControllerWidget(QSlider * slider, QString heading, int alignment, QWidget * parent, Qt::WindowFlags f) :
    QWidget(parent, f), m_slider(slider), m_value_label(new DisplayLabel(this)), m_heading_label(new HeaderLabel(heading,  this)),
    m_alignment(alignment)
{
    if(alignment == Qt::AlignRight)
        m_slider->setTickPosition(QSlider::TickPosition::TicksLeft);
    else if(alignment == Qt::AlignLeft)
        m_slider->setTickPosition(QSlider::TickPosition::TicksRight);

    init_layout();
    setFixedWidth(100);
    m_value_label->setFont(m_label_font);
    m_heading_label->setFont(m_label_font);

    QFontMetrics fm(m_label_font);
    m_value_label->setFixedHeight(fm.height()+15);
}

BaseSliderControllerWidget::~BaseSliderControllerWidget()
{

}

QSlider * BaseSliderControllerWidget::getSlider()
{
    return m_slider;
}

int BaseSliderControllerWidget::value() const
{
    return m_slider->value();
}

void BaseSliderControllerWidget::init_layout()
{
    QVBoxLayout * main_layout(new QVBoxLayout);

    // Slider and header
    {
        QHBoxLayout * layout(new QHBoxLayout);
        if(m_alignment == Qt::AlignLeft)
        {
            layout->addWidget(m_slider, 0, Qt::AlignLeft);
            layout->addWidget(m_heading_label, 0, Qt::AlignRight);
        }
        else
        {
            layout->addWidget(m_heading_label, 0, Qt::AlignLeft);
            layout->addWidget(m_slider, 0, Qt::AlignRight);
        }

        main_layout->addLayout(layout, 0);
    }
    main_layout->addWidget(m_value_label,0, Qt::AlignHCenter | Qt::AlignBottom);

    setLayout(main_layout);
}

//void BaseSliderControllerWidget::resize()
//{
//    int h(height());
//    m_slider->setFixedHeight(h-m_value_label->height());
//}

int BaseSliderControllerWidget::alignment()
{
    return m_alignment;
}

QString BaseSliderControllerWidget::format_two_digit_num(int digit)
{
    return QString("%1").arg(digit, 2, 10, QChar('0'));
}
