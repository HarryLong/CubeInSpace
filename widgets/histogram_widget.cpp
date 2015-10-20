#include "histogram_widget.h"
#include <QFont>
#include <QLabel>
#include <QPainter>
#include <QBoxLayout>

/********************
 * HISTOGRAM WIDGET *
 ********************/
const QFont HistogramWidget::_LABEL_FONT("Times", 10, QFont::Weight::Bold);
HistogramWidget::HistogramWidget(QString heading, QWidget * parent) : QWidget(parent),
    m_heading(heading), m_value(0), m_heading_lbl(new QLabel(heading, this))
{
    m_heading_lbl->setFont(HistogramWidget::_LABEL_FONT);
    init_layout();
}

HistogramWidget::~HistogramWidget()
{

}

void HistogramWidget::paintEvent(QPaintEvent * event)
{
    QPainter p(this);

    // Calculate color
    float green_ratio(m_value/100.f);
    float red_ratio(1-green_ratio);
    QColor fill_color(red_ratio*255, green_ratio*255, 0);

    int lbl_height(m_heading_lbl->height()*2);
    int w(width());
    int h(height()-lbl_height);

    int h_ratio(std::round((m_value/100.f) * h));

    QPoint top_left(0, h-h_ratio);
    QPoint bottom_right(w,h);
    QRect rect(top_left, bottom_right);
    p.fillRect(rect, fill_color);

    // Draw text
    QFontMetrics fm(p.font());
    QString value_txt(QString::number(m_value));
    value_txt.append("%");
    int text_h(fm.height());
    QRect text_rect(top_left, QPoint(w, top_left.y()+text_h));
    p.drawText(text_rect, Qt::AlignCenter, value_txt);

    p.end();
}

void HistogramWidget::setValue(int value)
{
    m_value = value;
}

void HistogramWidget::clear()
{
    setValue(0);
}

void HistogramWidget::init_layout()
{
    QVBoxLayout * main_layout = new QVBoxLayout;

    main_layout->addWidget(new QLabel, 1); // Padding

    {
        QHBoxLayout * layout = new QHBoxLayout;
        layout->addWidget(m_heading_lbl, 1, Qt::AlignCenter);
        main_layout->addLayout(layout,0);
    }

    setLayout(main_layout);
}
