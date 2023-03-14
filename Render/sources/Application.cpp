#include "Application.hpp"
#include "glm/glm.hpp"
#include <cstdlib>
#include <thread>
using namespace std::chrono_literals;

namespace SR {
Application::Application(uint32_t width, uint32_t height) : m_window(width, height) {}

void Application::setMeshes(std::vector<Mesh>&& meshes) noexcept { m_meshes = std::move(meshes); }

void Application::start() { drawLoop(); }

void Application::drawLine(const u32 x1, const u32 y1, const u32 x2, const u32 y2, Color color = Color{255}) {
    u32 max_x = std::max(x1, x2);
    u32 min_x = std::min(x1, x2);
    u32 max_y = std::max(y1, y2);
    u32 min_y = std::min(y1, y2);
    if (max_x - min_x > max_y - min_y) {
        float denominator = static_cast<float>(x1) - x2;
        float k = (static_cast<float>(y1) - y2) / denominator;
        float b = (static_cast<float>(x1) * y2 - x2 * y1) / denominator;
        for (u32 x = min_x; x < max_x; ++x) {
            float y = k * x + b;
            m_window.setPixel(x, y, color);
        }
    } else {
        float denominator = static_cast<float>(y1) - y2;
        float k = (static_cast<float>(x1) - x2) / denominator;
        float b = (static_cast<float>(x1) * y2 - x2 * y1) / denominator;
        for (u32 y = min_y; y < max_y; ++y) {
            float x = k * y - b; 
            m_window.setPixel(x, y, color);
        }
    }
}

void Application::drawTriangle(uVec2 p1, uVec2 p2, uVec2 p3, Color color) {
    drawLine(p1.x, p1.y, p2.x, p2.y, color);
    drawLine(p2.x, p2.y, p3.x, p3.y, color);
    drawLine(p3.x, p3.y, p1.x, p1.y, color);
}

void Application::drawLoop() {
    m_window.clearWindow(Color(5, 5, 5, 255));
    while (!m_window.windowShouldClose()) {
        m_window.setPixel(100, 100, {255, 255, 0, 0});
        m_window.setPixel(200, 100, {255, 0, 0, 0});
        m_window.setPixel(300, 100, {0, 0, 255, 0});
        Color Red{255, 0, 0, 255};
        drawTriangle({10, 10}, {1200, 400}, {250, 320}, Red);

        m_window.update();
    }
}
}  // namespace SR
