#include "orientation_widget.h"
#include <QPainter>
#include <QBrush>
#include <math.h>
#include <QAction>
#include <QBrush>
#include "geom.h"
#include "glm_rotations.h"

const glm::vec3 OrientationWidget::_BASE_NORTH_ORIENTATION = glm::vec3(0,0,-1);

OrientationWidget::OrientationWidget(float pitch, float yaw, QAction * edit_mode_trigger_action, QWidget * parent, Qt::WindowFlags f) :QWidget(parent, f),
    m_font("times", 40, QFont::Bold), m_font_metrics(m_font), m_north_yaw(0), m_edit_mode(false), m_north_orientation(OrientationWidget::_BASE_NORTH_ORIENTATION),
    m_edit_mode_trigger_action(edit_mode_trigger_action), m_edit_mode_brush(QColor(0, 250, 0, 150)), m_default_brush(QColor(100, 100, 100, 150))
{
    setAutoFillBackground(true);
    init_pens();
    establish_connections();
    setCameraOrientation(pitch, yaw);
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
    QWidget::paintEvent(event);
    QPainter painter(this);

    int w (width());
    int h (height());
    int line_seperation(w/(2*HALF_SECONDARY_LINE_COUNT)-1);

    float slide ( w-((fmod(normalize(m_camera_yaw+m_north_yaw+45.0),90.0) / 90.0f) * w) );

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
        QChar orientation_char(OrientationWidget::angle_to_letter(normalize(m_camera_yaw+m_north_yaw)));
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

void OrientationWidget::setCameraOrientation(float pitch, float yaw)
{
    m_pitch = pitch;
    m_camera_yaw = yaw;

    repaint();
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
    if(angle >= 45.0f & angle < 135.f)
        return QChar('E');
    else if (angle >= 135.f && angle < 225)
        return QChar('S');
    else if (angle >= 225 && angle < 315)
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

void OrientationWidget::establish_connections()
{
    connect(m_edit_mode_trigger_action, SIGNAL(triggered(bool)), this, SLOT(toggle_edit_mode(bool)));
}

void OrientationWidget::rotateNorth(int amount)
{
    m_north_yaw += amount;
    Geom::normalizeDegrees(m_north_yaw);

    m_north_orientation = glm::rotateY(OrientationWidget::_BASE_NORTH_ORIENTATION, Geom::toRadians(m_north_yaw));

    update();

    emit northOrientationChanged(m_north_orientation[0], m_north_orientation[1], m_north_orientation[2]);
}

float OrientationWidget::normalize(const float & degrees)
{
    float ret(degrees);
    Geom::normalizeDegrees(ret);
    return ret;
}
