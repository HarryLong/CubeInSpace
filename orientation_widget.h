#ifndef ORIENTATION_WIDGET_H
#define ORIENTATION_WIDGET_H

#include <QWidget>
#include <QPen>
#include <QPainter>
#include <QChar>
#include <glm/common.hpp>

class OrientationWidget : public QWidget
{
    Q_OBJECT
public:
    OrientationWidget(float pitch, float yaw, QAction * edit_mode_trigger_action, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~OrientationWidget();

    bool editMode();

    const static glm::vec3 _BASE_NORTH_ORIENTATION;

signals:
    void northOrientationChanged(float north_orientation_x, float north_orientation_y, float north_orientation_z);

public slots:
    void setCameraOrientation(float pitch, float yaw);
    void saveOrientation();
    void rotateNorth(int amount);

protected:
    virtual void paintEvent(QPaintEvent * event);

private slots:
    void toggle_edit_mode(bool checked);

private:
    void init_pens();
    void establish_connections();
    static QChar angle_to_letter(float angle);
    float normalize(const float & degrees);

    float m_pitch;

    float m_camera_yaw;

    float m_north_yaw;

    QPen m_primary_line_pen;
    QPen m_text_drawing_pen;
    QPen m_secondary_line_pen;
    QFont m_font;
    QFontMetrics m_font_metrics;

    bool m_edit_mode;

    QAction * m_edit_mode_trigger_action;

    QBrush m_edit_mode_brush;
    QBrush m_default_brush;

    glm::vec3 m_north_orientation;
};


#endif // ORIENTATION_WIDGET_H
