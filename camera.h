#ifndef CAMERA_H
#define CAMERA_H

#include "glm_wrapper.h"
#include "controllers.h"
#include <QObject>
class QTimer;
class Camera : public QObject{
    Q_OBJECT
public:
    enum Direction {
        UP, DOWN, LEFT, RIGHT, FORWARD, BACK
    };
    enum Type {
        ORTHO, FREE
    };

    Camera(ViewControllers view_controllers);
    ~Camera();

    void reset();

    //Given a specific moving direction, the camera will be moved in the appropriate direction
    //For a spherical camera this will be around the look_at point
    //For a free camera a delta will be computed for the direction of movement.
    void move(Camera::Direction dir);
    //Change the yaw and pitch of the camera based on the 2d movement of the mouse
    void rotate(float x, float y);

    //Setting Functions
    //Changes the camera mode, only three valid modes, Ortho, Free, and Spherical
    void setType(Type cam_mode);
    //Set the position of the camera
    void setPosition(glm::vec3 pos);
    //Set's the look at point for the camera
    void lookAt(glm::vec3 pos);
    //Changes the Field of View (FOV) for the camera
    void setFOV(double fov);
    //Change the viewport location and size
    void setAspect(double m_aspect);
    //Change the clipping distance for the camera
    void setClipping(double near_clip_distance, double far_clip_distance);
    glm::vec3 getCameraDirection() const;

    //Getting Functions
    Camera::Type getType() const;
    //		void GetViewport(int &loc_x, int &loc_y, int &width, int &height);
    void getTransforms(glm::mat4 & proj, glm::mat4 & view);

signals:
    void cameraDirectionChanged(float camera_direction_x, float camera_directiony, float camera_direction_z);

private slots:
    void setTranslationScale(int scale);
    void setRotationScale(int scale);
    void update();

private:
    //This function updates the camera
    //Depending on the current camera mode, the projection and viewport matricies are computed
    //Then the position and location of the camera is updated
    void init_connections();
    //Change the pitch (up, down) for the free camera
    void pitch(float radians);
    //Change yaw (left, right) for the free camera
    void yaw(float radians);
    void emit_camera_direction_changed();

    Camera::Type camera_type;

    double m_aspect;
    double m_fov;
    double m_near_clip;
    double m_far_clip;

    float m_camera_yaw;
    float m_camera_pitch;

    glm::vec3 m_camera_position;
    glm::vec3 m_camera_position_delta;
    glm::vec3 m_camera_look_at;
    glm::vec3 m_camera_direction;

    glm::vec3 m_camera_up;

    glm::mat4 m_proj;
    glm::mat4 m_view;
    glm::mat4 m_MVP;

    float m_translation_scale;
    float m_rotation_scale;

    ViewControllers m_view_controllers;

    QTimer * m_timer;

    static const float _MAX_TRANSLATION_SCALE;
    static const float _MAX_ROTATION_SCALE;
};
#endif
