#include "Application.hpp"
//
namespace SR {
Application::Application(uint32_t width, uint32_t height) : m_window(width, height) {}

Application::~Application() {}

void Application::start() { drawLoop(); }

void Application::drawLoop() {
    while (!m_window.windowShouldClose()) {
        m_window.clearWindow({255, 0, 255, 0});
        m_window.setPixel(100, 100, {255, 255, 0, 0});
        m_window.setPixel(200, 100, {255, 0, 0, 0});
        m_window.setPixel(300, 100, {0, 0, 255, 0});
        m_window.update();
    }
}
}  // namespace SR
