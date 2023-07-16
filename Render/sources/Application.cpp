#include "Application.hpp"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <array>
#include <type_traits>
#include <algorithm>
#include <thread>

namespace SR {

Application::Application(uint32_t width, uint32_t height, bool exportImage)
    : m_window(width, height), m_camera(m_window.getExtent()[0], m_window.getExtent()[1]), m_exportImage(exportImage), m_render(m_window) {}

void Application::setMeshes(std::vector<Mesh>&& meshes) noexcept { m_meshes = std::move(meshes); }

void Application::processEvent(const Window::WindowEvent e) {
    const float step = 0.025f;
    switch (e) {
        case Window::WindowEvent::KEY_PRESSED_A: 
            m_camera.changeCameraPosition(glm::vec3(1.f * step, 0.f, 0.f)); break;
        case Window::WindowEvent::KEY_PRESSED_D: 
            m_camera.changeCameraPosition(glm::vec3(-1.f * step, 0.f, 0.f)); break;
        case Window::WindowEvent::KEY_PRESSED_W: 
            m_camera.changeCircleHeight(1.f); break;
        case Window::WindowEvent::KEY_PRESSED_S: 
            m_camera.changeCircleHeight(-1.f); break;
        case Window::WindowEvent::KEY_PRESSED_Q: m_camera.changeCirclePos(-5.f * 3.141592f / 180.f); break;
        case Window::WindowEvent::KEY_PRESSED_E: m_camera.changeCirclePos(5.f * 3.141592f / 180.f); break;
        case Window::WindowEvent::KEY_PRESSED_SPACE: m_camera.loadDefaultCircleCamera(); break;
        default: break;
    }
}

void Application::start() {
    auto [width, height] = m_window.getExtent();
    m_camera.setViewCircleCamera(15.f, 12.f);
    m_camera.setPerspectiveProjection(glm::radians(m_camera.getZoom()), static_cast<float>(width) / height,
                                      0.1f, 256.f);
    drawLoop(); 
}

inline std::array<glm::uvec2, 2> findBoundingBox(uVec2 p1, uVec2 p2, uVec2 p3) noexcept {
    uVec2 leftDownPoint = {
        std::min({p1.x, p2.x, p3.x}),
        std::min({p1.y, p2.y, p3.y})};
    uVec2 topRightPoint = {
        std::max({p1.x, p2.x, p3.x}),
        std::max({p1.y, p2.y, p3.y})};
    return {leftDownPoint, topRightPoint};
}

void Application::drawMeshes() { 
    glm::mat4 view = m_camera.getView(); 
    glm::mat4 projection = m_camera.getProjection();
    glm::mat4 screen = m_camera.getNDCtoScreenMatrix();
    m_render.setNDCtoScreenMatrix(screen);

    for (auto& mesh : m_meshes) { 
        glm::mat4 model = mesh.getModelMatrix();
        glm::mat4 MVP = projection * view * model;

        m_render.setMVPmatrix(std::move(MVP));
        m_render.setVertexData(mesh.getVertexData());
        m_render.setIndexData(mesh.getIndicesData());

        m_render.draw();
    }
}

void Application::updateCameraPosition() {
    auto now = std::chrono::system_clock::now();
    static auto last_time = now;
    auto diff = now - last_time;
    m_camera.changeCirclePos(0.005f * diff.count()/1000.f * 3.141592f / 180.f);   
    last_time = now;
}

void Application::drawLoop() {
    Color background = Color(5, 5, 5, 255);
    m_window.clearWindow(background);
    drawMeshes();
    m_window.updateScreen();
    if (m_exportImage)
        m_window.exportToBMP();

    m_render.setExportBlobs(false);

    while (!m_window.windowShouldClose()) {
        Window::WindowEvent e = m_window.updateEvents();
        processEvent(e);
        if (e != Window::WindowEvent::DEFAULT) {
            m_window.clearWindow(background);
            drawMeshes();
            m_window.updateScreen();
            
#ifndef FORCE_BUFFER_WINDOW
            updateCameraPosition();
#endif  // FORCE_BUFFER_WINDOW
#ifdef PROFILE_SPEED
            static auto start = std::chrono::system_clock::now();
            auto end = std::chrono::system_clock::now();
            std::cout << "Frame time, ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                      << "\nFPS: "
                      << 1'000'000'000.0 / std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
                      << "\n" << std::endl;
            start = end;
#endif  // PROFILE_SPEED

#ifndef NDEBUG
            std::cout << "Camera: {" << m_camera.getCameraPos().x << "," << m_camera.getCameraPos().y << ","
                                  << m_camera.getCameraPos().z << "}" << std::endl;
            std::cout << std::endl;
#endif
        }
    }
}
}  // namespace SR
