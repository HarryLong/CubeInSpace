//camera.cpp
#include "camera.h"
#include "geom.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <QTimer>
#include <QDebug>

const float Camera::_MAX_TRANSLATION_SCALE = 5.0f;
const float Camera::_MAX_ROTATION_SCALE = 2.0f;
Camera::Camera() :
    camera_type(Camera::Type::FREE),
    m_camera_up(glm::vec3(0, 1, 0)),
    m_fov(M_PI/2.0),
    m_camera_position(0,5,5),
    m_camera_look_at(0,0,0),
    m_camera_direction(glm::normalize(m_camera_look_at - m_camera_position)),
    m_camera_position_delta(0, 0, 0),
    m_near_clip(.1),
    m_far_clip(1000),
    m_camera_yaw(0),
    m_camera_pitch(0),
    m_timer(new QTimer)
{
    init_connections();
    setTranslationSensitivity(1);
    setRotationSensitivity(1);
    m_timer->start(1000/60);
}

Camera::~Camera()
{
    m_timer->stop();
    delete m_timer;
}

void Camera::reset() {
    m_camera_up = glm::vec3(0, 1, 0);
}

void Camera::update() {
    m_camera_direction = glm::normalize(m_camera_look_at - m_camera_position);
	//need to set the matrix state. this is only important because lighting doesn't work if this isn't done
//	glViewport(viewport_x, viewport_y, window_width, window_height);

    if (camera_type == ORTHO) {
		//our projection matrix will be an orthogonal one in this case
		//if the values are not floating point, this command does not work properly
		//need to multiply by aspect!!! (otherise will not scale properly)
        m_proj = glm::ortho(-1.5f * float(m_aspect), 1.5f * float(m_aspect), -1.5f, 1.5f, -10.0f, 10.f);
    } else if (camera_type == FREE) {
        m_proj = glm::perspective(m_fov, m_aspect, m_near_clip, m_far_clip);
        //detmine axis for pitch rotation
        glm::vec3 left_right_axis = glm::cross(m_camera_direction, m_camera_up);
        //compute quaternion for pitch based on the camera pitch angle
        glm::quat pitch_quat = glm::angleAxis(m_camera_pitch, left_right_axis);
        //determine heading quaternion from the camera up vector and the heading angle
        glm::quat yaw_quat = glm::angleAxis(m_camera_yaw, m_camera_up);
        //add the two quaternions
        glm::quat temp = glm::cross(pitch_quat, yaw_quat);
        temp = glm::normalize(temp);
        //update the direction from the quaternion
        m_camera_direction = glm::rotate(temp, m_camera_direction);
        //add the camera delta
        m_camera_position += m_camera_position_delta;
        //set the look at to be infront of the camera
        m_camera_look_at = m_camera_position + m_camera_direction * 1.0f;
        //damping for smooth camera
        m_camera_yaw *= .5;
        m_camera_pitch *= .5;
        m_camera_position_delta = m_camera_position_delta * .8f;
	}
	//compute the MVP
    m_view = glm::lookAt(m_camera_position, m_camera_look_at, m_camera_up);
    m_MVP = m_proj * m_view;
}

//Setting Functions
void Camera::setType(Type cam_mode) {
    camera_type = cam_mode;
    reset();
}

void Camera::setPosition(glm::vec3 pos) {
    m_camera_position = pos;
}

void Camera::lookAt(glm::vec3 pos) {
    m_camera_look_at = pos;
}

void Camera::setFOV(double fov) {
    m_fov = fov;
}

void Camera::setAspect(double aspect)
{
    m_aspect = aspect;
}

void Camera::setClipping(double near_clip_distance, double far_clip_distance) {
    m_near_clip = near_clip_distance;
    m_far_clip = far_clip_distance;
}

void Camera::move(Direction dir) {
    move(dir, 1);
}

void Camera::move(Direction dir, float amount) {
    if (camera_type == FREE) {
        switch (dir) {
            case UP:
                m_camera_position_delta += m_camera_up * amount * m_translation_sensitivity;
                break;
            case DOWN:
                m_camera_position_delta -= m_camera_up * amount * m_translation_sensitivity;
                break;
            case LEFT:
                m_camera_position_delta -= glm::cross(m_camera_direction, m_camera_up) * amount * m_translation_sensitivity;
                break;
            case RIGHT:
                m_camera_position_delta += glm::cross(m_camera_direction, m_camera_up) * amount * m_translation_sensitivity;
                break;
            case FORWARD:
                m_camera_position_delta += m_camera_direction * amount * m_translation_sensitivity;
                break;
            case BACK:
                m_camera_position_delta -= m_camera_direction * amount * m_translation_sensitivity;
                break;
        }
    }
}

void Camera::pitch(float radians) {
    m_camera_pitch += radians;
}

void Camera::yaw(float radians) {
    m_camera_yaw += radians;
}

void Camera::rotate(float x, float y) {
    yaw(-m_rotation_sensitivity * x);
    pitch(-m_rotation_sensitivity * y);
    emit_camera_direction_changed();
}

Camera::Type Camera::getType() const
{
    return camera_type;
}

void Camera::getTransforms(glm::mat4 &proj, glm::mat4 &view)
{
    proj = m_proj;
    view = m_view;
}

void Camera::setTranslationSensitivity(float sensitivity)
{
    m_translation_sensitivity = sensitivity * _MAX_TRANSLATION_SCALE;
}

void Camera::setRotationSensitivity(float sensitivity)
{
    m_rotation_sensitivity = sensitivity * _MAX_ROTATION_SCALE;
}

void Camera::init_connections()
{
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
}

glm::vec3 Camera::getCameraDirection() const
{
    return m_camera_direction;
}

void Camera::emit_camera_direction_changed()
{
    emit cameraDirectionChanged(m_camera_direction.x, m_camera_direction.y, m_camera_direction.z);
}
