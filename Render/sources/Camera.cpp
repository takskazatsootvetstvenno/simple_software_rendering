#include "Camera.hpp"

namespace SR {
Camera::Camera(uint32_t screenWidth, uint32_t screenHeight)
{
    updateScreenSize(screenWidth, screenHeight);
}

void Camera::loadDefaultCircleCamera() {
    m_circle_pos = 0.f;
    m_circle_radius = 1.f;
    m_circle_height = 0.f;
    m_zoom = 45.f;
    setViewCircleCamera(15.f, 5.f);
}

void Camera::setPerspectiveProjection(float fovy, float aspect, float near, float far) {
    m_projection = glm::perspective(fovy, aspect, near, far);
}

void Camera::setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
    m_cameraPosition = position;
    m_view = glm::lookAt(position, target, up);
}

void Camera::setViewCircleCamera(const float radius, const float height) {
    m_circle_radius = radius;
    m_circle_height = height;
    setViewTarget(glm::vec3(m_circle_radius * cos(m_circle_pos), m_circle_height, m_circle_radius * sin(m_circle_pos)),
                  glm::vec3(0.f, 1.f, 0.f));
}
void Camera::changeCirclePos(const float add_value) {
    m_circle_pos += add_value;
    setViewTarget(glm::vec3(m_circle_radius * cos(m_circle_pos), m_circle_height, m_circle_radius * sin(m_circle_pos)),
                  glm::vec3(0.f, 1.f, 0.f));
}
void Camera::changeCircleHeight(const float add_value) {
    m_circle_height += add_value;
    setViewTarget(glm::vec3(m_circle_radius * cos(m_circle_pos), m_circle_height, m_circle_radius * sin(m_circle_pos)),
                  glm::vec3(0.f, 1.f, 0.f));
}

void Camera::updateScreenSize(uint32_t screenWidth, uint32_t sreenHeight) noexcept {
    m_screenWidth = screenWidth;
    m_screenHeight = sreenHeight;
    float width = m_screenWidth;
    float height = m_screenHeight;
    m_NDCtoScreen = {
        (width - 1.f) / 2.f, 0, 0, 0,
        0, -(height - 1.f) / 2.f, 0, 0,
        0, 0, 1, 0,
        (width - 1.f) / 2.f, (height - 1.f) / 2.f, 0, 1};
}

void Camera::changeCameraPosition(glm::vec3 addPos) {
    m_cameraPosition += addPos;
    setViewTarget(m_cameraPosition, glm::vec3{0.f});
}

const glm::mat4& Camera::getNDCtoScreenMatrix() const noexcept { return m_NDCtoScreen; }

const glm::mat4& Camera::getProjection() const noexcept { return m_projection; }

const glm::mat4& Camera::getView() const noexcept { return m_view; }

const glm::vec3& Camera::getCameraPos() const noexcept { return m_cameraPosition; }

float Camera::getZoom() const noexcept { return m_zoom; }

}