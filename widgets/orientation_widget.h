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
    enum Orientation{
        NORTH,
        EAST,
        SOUTH,
        WEST
    };

    OrientationWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~OrientationWidget();

    bool editMode();

    const static glm::vec3 _BASE_NORTH_ORIENTATION;

signals:
    void northOrientationChanged(float north_orientation_x, float north_orientation_y, float north_orientation_z);

public slots:
    void setCameraDirection(glm::vec3 direction);
    void setCameraDirection(float camera_direction_x, float camera_direction_y, float camera_direction_z);
    void saveOrientation();
    void rotateNorth(Orientation orientation);

protected:
    virtual void paintEvent(QPaintEvent * event);

private slots:
    void toggle_edit_mode(bool checked);

private:
    void init_pens();
    static QChar angle_to_letter(float angle);
    float normalize(const float & degrees);

    QPen m_primary_line_pen;
    QPen m_text_drawing_pen;
    QPen m_secondary_line_pen;
    QFont m_font;
    QFontMetrics m_font_metrics;

    bool m_edit_mode;

    QAction * m_edit_mode_trigger_action;

    QBrush m_edit_mode_brush;
    QBrush m_default_brush;

    glm::vec3 m_camera_direction;
    glm::vec3 m_north_direction;
};


#endif // ORIENTATION_WIDGET_H
