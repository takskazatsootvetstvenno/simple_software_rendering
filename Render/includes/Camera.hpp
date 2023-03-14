#include <glm/glm.hpp>

namespace SR {

class Camera {
 public:
    Camera() = default;

    void setPerspectiveProjection(float fovy, float aspect, float near, float far);
    void loadDefaultCameraPosition() noexcept;
    const glm::mat4& getProjection() const noexcept;
    const glm::mat4& getView() const noexcept;
    const glm::vec3& getCameraPos() const noexcept;

 private:
    glm::mat4 m_projection{1.f};
    glm::mat4 m_view{1.f};
    glm::vec3 m_cameraPosition{0.f};
};
}