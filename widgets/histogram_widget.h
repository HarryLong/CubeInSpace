#ifndef HISTOGRAM_WIDGET_H
#define HISTOGRAM_WIDGET_H

#include <QWidget>

class QLabel;
class HistogramWidget : public QWidget
{
Q_OBJECT
public:
    HistogramWidget(QString heading, QWidget * parent = 0);
    ~HistogramWidget();

    void setValue(int value);
    void clear();

protected:
    void paintEvent(QPaintEvent * event) override;

private:
    void init_layout();
    int m_value;
    static const QFont _LABEL_FONT;
    QString m_heading;
    QLabel * m_heading_lbl;
};

#endif
