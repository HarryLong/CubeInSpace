#include "orientation_widget.h"
#include <QPainter>
#include <QBrush>
#include <math.h>
#include <QAction>
#include <QBrush>
#include "geom.h"
#include "../gl_core/glm_wrapper.h"

const glm::vec3 OrientationWidget::_BASE_NORTH_ORIENTATION = glm::vec3(0,0,-1);

OrientationWidget::OrientationWidget(QWidget * parent, Qt::WindowFlags f) : QWidget(parent, f),
    m_font("times", 40, QFont::Bold), m_font_metrics(m_font), m_edit_mode(false), m_north_direction(OrientationWidget::_BASE_NORTH_ORIENTATION),
    m_edit_mode_brush(QColor(0, 250, 0, 150)), m_default_brush(QColor(100, 100, 100, 150)), m_camera_direction(0,0,-1)
{
    setAutoFillBackground(true);
    init_pens();
    toggle_edit_mode(false);
}

OrientationWidget::~OrientationWidget()
{

}

bool OrientationWidget::editMode()
{
    return m_edit_mode;
}

void OrientationWidget::saveOrientation()
{
    m_edit_mode = false;
    update();
}

#include <iostream>
#define HALF_SECONDARY_LINE_COUNT 5
#define PRIMARY_VERTICAL_SPLIT 0
#define SECONDARY_VERTICAL_SPLIT 4
#include <QPaintEvent>
void OrientationWidget::paintEvent(QPaintEvent * event)
{
    QWidget::paintEvent(event); // Don't call this as it will trigger a redraw on the render window

    int w (width());
    int h (height());

    if(w != 0 && h != 0)
    {
        QPainter painter(this);

        int line_seperation(w/(2*HALF_SECONDARY_LINE_COUNT)-1);

        double dot_product( glm::dot(m_camera_direction, m_north_direction) );
        glm::vec3 cross_product( glm::cross(m_camera_direction, m_north_direction) );

        float separation_angle ( std::acos(dot_product) );

        // Check if separation > 180 degrees
        if(cross_product.y < 0)
            separation_angle = 2*M_PI - separation_angle;

        float slide ( w-((fmod(separation_angle+M_PI_4,M_PI_2) / M_PI_2) * w) );

        // Primary Line
        int start_x(slide); // TODO: Change this to reflect current yaw
        {
            int y1(PRIMARY_VERTICAL_SPLIT);
            int y2(h-PRIMARY_VERTICAL_SPLIT);
            QPoint p1(start_x, y1);
            QPoint p2(start_x, y2);

            painter.setPen(m_primary_line_pen);
            painter.drawLine(p1,p2);
        }

        // Draw the text (N, E, S, W)
        {
            QChar orientation_char(OrientationWidget::angle_to_letter(separation_angle));
            int pixels_wide(m_font_metrics.width(orientation_char));

            painter.setPen(m_text_drawing_pen);

            QRect bounding_rect(start_x+m_primary_line_pen.width()+1, 0, pixels_wide, height());
            painter.drawText(bounding_rect, Qt::AlignLeft, orientation_char);
        }

        // Secondary Lines
        {
            painter.setPen(m_secondary_line_pen);
            int y1(SECONDARY_VERTICAL_SPLIT);
            int y2(h-SECONDARY_VERTICAL_SPLIT);

            // Right of secondary line
            for(int x(start_x+line_seperation); x <= w; x+= line_seperation)
            {
                QPoint p1(x, y1);
                QPoint p2(x, y2);

                painter.drawLine(p1, p2);
            }
            // Left of secondary line
            for(int x(start_x-line_seperation); x >= 0; x -= line_seperation)
            {
                QPoint p1(x, y1);
                QPoint p2(x, y2);

                painter.drawLine(p1, p2);
            }
        }

        painter.end();
    }
}

void OrientationWidget::setCameraDirection(glm::vec3 direction)
{
    m_camera_direction = glm::normalize(glm::vec3(direction[0], 0, direction[2]));
    update();
}

void OrientationWidget::setCameraDirection(float camera_direction_x, float camera_direction_y, float camera_direction_z)
{
    setCameraDirection(glm::vec3(camera_direction_x, camera_direction_y, camera_direction_z));
}

void OrientationWidget::init_pens()
{
    m_primary_line_pen.setStyle(Qt::SolidLine);
    m_primary_line_pen.setWidth(3);
    m_primary_line_pen.setBrush(Qt::red);
    m_primary_line_pen.setCapStyle(Qt::RoundCap);
    m_primary_line_pen.setJoinStyle(Qt::RoundJoin);

    m_text_drawing_pen.setStyle(Qt::SolidLine);
    m_text_drawing_pen.setWidth(1);
    m_text_drawing_pen.setBrush(Qt::red);
    m_text_drawing_pen.setCapStyle(Qt::RoundCap);
    m_text_drawing_pen.setJoinStyle(Qt::RoundJoin);

    m_secondary_line_pen.setStyle(Qt::SolidLine);
    m_secondary_line_pen.setWidth(1);
    m_secondary_line_pen.setBrush(Qt::white);
    m_secondary_line_pen.setCapStyle(Qt::RoundCap);
    m_secondary_line_pen.setJoinStyle(Qt::RoundJoin);
}

QChar OrientationWidget::angle_to_letter(float angle)
{
    if(angle >= M_PI_4 & angle < M_PI_4*3)
        return QChar('E');
    else if (angle >= M_PI_4*3 && angle < M_PI_4*5)
        return QChar('S');
    else if (angle >= M_PI_4*5 && angle < M_PI_4*7)
        return QChar('W');
    else
        return QChar('N');
}

void OrientationWidget::toggle_edit_mode(bool checked)
{
    QPalette pal = palette();
    pal.setBrush(QPalette::Window, checked ? m_edit_mode_brush : m_default_brush);
    setPalette(pal);

    m_edit_mode = checked;

    update();
}

void OrientationWidget::rotateNorth(Orientation orientation)
{
    float rotation = 0;
    if(orientation == Orientation::EAST)
    {
        rotation = (-2 * M_PI / 360.0);
    }
    else if(orientation == Orientation::WEST)
    {
        rotation = (2 * M_PI / 360.0);
    }
    m_north_direction = glm::normalize(glm::rotateY(m_north_direction, rotation));
    update();
    emit northOrientationChanged(m_north_direction[0], m_north_direction[1], m_north_direction[2]);
}

float OrientationWidget::normalize(const float & degrees)
{
    float ret(degrees);
    Geom::normalizeDegrees(ret);
    return ret;
}
