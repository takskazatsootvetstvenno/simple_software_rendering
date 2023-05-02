#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace SR {

class Camera {
 public:
    Camera(uint32_t screenWidth, uint32_t sreenHeight);

    void setPerspectiveProjection(float fovy, float aspect, float near, float far);
    void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = {0.f, 1.f, 0.f});
    void setViewCircleCamera(const float radius, const float height);
    void changeCirclePos(const float add_value);
    void changeCircleHeight(const float add_value);
    void updateScreenSize(uint32_t screenWidth, uint32_t sreenHeight) noexcept;
    void changeCameraPosition(glm::vec3 addPos);

    const glm::mat4& getNDCtoScreenMatrix() const noexcept;
    const glm::mat4& getProjection() const noexcept;
    const glm::mat4& getView() const noexcept;
    const glm::vec3& getCameraPos() const noexcept;

    float getZoom() const noexcept;
    void loadDefaultCircleCamera();
 private:
    glm::mat4 m_projection{1.f};
    glm::mat4 m_view{1.f};
    glm::mat4 m_NDCtoScreen{1.f};
    glm::vec3 m_cameraPosition{0.f};
    float m_circle_pos = 0.f;
    float m_circle_radius = 1.f;
    float m_circle_height = 0.f;
    float m_zoom = 45.f;               //vertical field of view
    uint32_t m_screenWidth;
    uint32_t m_screenHeight;

};
}