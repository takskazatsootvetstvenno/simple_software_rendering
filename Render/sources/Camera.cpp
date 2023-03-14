#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"

namespace SR {

void Camera::setPerspectiveProjection(float fovy, float aspect, float near, float far) {
    m_projection = glm::perspective(fovy, aspect, near, far);
}

void Camera::loadDefaultCameraPosition() noexcept {
    m_cameraPosition = glm::vec3{1.f};
}

const glm::mat4& Camera::getProjection() const noexcept { return m_projection; }

const glm::mat4& Camera::getView() const noexcept { return m_view; }

const glm::vec3& Camera::getCameraPos() const noexcept { return m_cameraPosition; }



}