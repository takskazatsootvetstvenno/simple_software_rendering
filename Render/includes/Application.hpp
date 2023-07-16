#pragma once
#include <cstdint>
#include <vector>

#include "Window.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Render.hpp"

namespace SR {
    using u32 = uint32_t;
    using uVec2 = glm::u32vec2;

class Application {
 public:
    using Color = glm::u8vec4;
    Application(uint32_t width, uint32_t height, bool exportImage = true);
    void setMeshes(std::vector<Mesh>&& meshes) noexcept;
    void processEvent(const Window::WindowEvent e);
    void start();
    void drawMeshes();
    void drawLoop();
    void updateCameraPosition();
 private:
    Window m_window;
    Camera m_camera;
    Render m_render;
    std::vector<Mesh> m_meshes;
    bool m_exportImage;
};
}  // namespace SR
