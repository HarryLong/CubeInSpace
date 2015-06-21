#ifndef ORIENTATION_WIDGET_H
#define ORIENTATION_WIDGET_H

#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QChar>

class OrientationWidget : public QWidget
{
    Q_OBJECT
public:
    OrientationWidget(float pitch, float yaw, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~OrientationWidget();

public slots:
    void setCameraOrientation(float pitch, float yaw);
    void triggerEditMode();
    void saveOrientation();

protected:
    virtual void paintEvent(QPaintEvent * event);

private:
    void init_pens();
    static QChar angle_to_letter(float angle);

    float m_pitch;

    float m_north_rotation;

    float m_yaw;

    QPen m_primary_line_pen;
    QPen m_text_drawing_pen;
    QPen m_secondary_line_pen;
    QFont m_font;
    QFontMetrics m_font_metrics;

    bool m_orientation_edit_mode;
};


#endif // ORIENTATION_WIDGET_H
