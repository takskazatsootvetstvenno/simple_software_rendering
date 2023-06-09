#include <cstdint>
#include <vector>

#include "Window.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"

namespace SR {
class Application {
 public:
    using Color = glm::u8vec4;
    using u32 = uint32_t;
    using uVec2 = glm::u32vec2;
    Application(uint32_t width, uint32_t height, bool exportImage = true);
    void setMeshes(std::vector<Mesh>&& meshes) noexcept;
    void processEvent(const Window::WindowEvent e);
    void start();
    template<typename ColorT>
    void drawLine(const u32 x1, const u32 y1, const u32 x2, const u32 y2, ColorT color) noexcept;
    void drawTriangle(uVec2 p1, uVec2 p2, uVec2 p3, Color color) noexcept;
    void drawMeshes();
    void drawLoop();
    void updateCameraPosition();
 private:
    Window m_window;
    Camera m_camera;
    std::vector<Mesh> m_meshes;
    bool m_exportImage;
};
}  // namespace SR
