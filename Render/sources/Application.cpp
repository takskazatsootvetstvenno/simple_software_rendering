#include "Application.hpp"
#include <iostream>
#include <cstdlib>
#include <chrono>

namespace SR {
Application::Application(uint32_t width, uint32_t height, bool exportImage)
    : m_window(width, height), m_camera(m_window.getExtent()[0], m_window.getExtent()[1]), m_exportImage(exportImage) {}

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
    }
}

void Application::start() {
    auto [width, height] = m_window.getExtent();
    m_camera.setViewCircleCamera(15.f, 12.f);
    m_camera.setPerspectiveProjection(glm::radians(m_camera.getZoom()), static_cast<float>(width) / height,
                                      0.1f, 256.f);
    drawLoop(); 
}

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

void Application::drawMeshes() { 
    glm::mat4 view = m_camera.getView(); 
    glm::mat4 projection = m_camera.getProjection();
    glm::mat4 screen = m_camera.getNDCtoScreenMatrix();
    Color redColor{255, 0, 0, 255};
    size_t unclipped_vertices = 0;
    for (auto& mesh : m_meshes) { 
        auto& transformed_vertices = mesh.getVertexStageBuffer();

        auto& inputVertexMas = mesh.getVertexData();
        auto model = mesh.getModelMatrix();
        glm::mat4 MVP = projection * view * model;
        for (auto vertex_index : mesh.getIndicesData())
        {  
            auto& resultPos = inputVertexMas[vertex_index].position;
            auto pos = MVP * glm::vec4(resultPos, 1.f);
            bool isClipped = false;
            if ((pos.x > pos.w || pos.x < -pos.w) ||
                (pos.y > pos.w || pos.y < -pos.w) ||
                (pos.z > pos.w || pos.z < -pos.w))
            {
                isClipped = true;
            }
            if (isClipped == false)
            {
               ++unclipped_vertices;
            }
            transformed_vertices.emplace_back(pos, isClipped);
        }

        //transformed_vertices contains a ndc vertices
        for (auto& vertex : transformed_vertices)
        {
            if (vertex.second) continue;
            auto& v = vertex.first;
            float const w_reciprocal{1.0f / v.w};
            v.x *= w_reciprocal;
            v.y *= w_reciprocal;
            v.z *= w_reciprocal;
            v.w = 1;

            v = screen * v;
        }
        // transformed_vertices contains 2d vertices after perpective division

        for (size_t i = 0; i < transformed_vertices.size(); i += 3) {
            bool v1_clipped = transformed_vertices[i].second;
            bool v2_clipped = transformed_vertices[i + 1].second;
            bool v3_clipped = transformed_vertices[i + 2].second;

            auto v1 = transformed_vertices[i].first;
            auto v2 = transformed_vertices[i + 1].first;
            auto v3 = transformed_vertices[i + 2].first;
            if (v1.x < 0 || v2.x < 0 || v3.x < 0 || v1.y < 0 || v2.y < 0 || v3.y < 0) continue;
            if (!v1_clipped && !v2_clipped) {
                drawLine(
                    static_cast<u32>(v1.x), static_cast<u32>(v1.y),
                    static_cast<u32>(v2.x), static_cast<u32>(v2.y),
                    redColor);
            }
            if (!v2_clipped && !v3_clipped) {
                drawLine(
                    static_cast<u32>(v2.x), static_cast<u32>(v2.y),
                    static_cast<u32>(v3.x), static_cast<u32>(v3.y),
                    redColor);
            }
            if (!v3_clipped && !v1_clipped) {
                drawLine(
                    static_cast<u32>(v3.x), static_cast<u32>(v3.y),
                    static_cast<u32>(v1.x), static_cast<u32>(v1.y),
                    redColor);
            }
        }
    }
#ifndef NDEBUG
    size_t all_vertices = 0;
    for (auto mesh : m_meshes) { all_vertices += mesh.getVertexData().size(); }
    std::cout << "Drawed " << unclipped_vertices << "(" << all_vertices << ")" << std::endl;
#endif  // DEBUG
}

void Application::updateCameraPosition() {
    auto now = std::chrono::system_clock::now();
    static auto last_time = now;
    std::time_t end_time = std::chrono::system_clock::to_time_t(now);
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
#ifndef NDEBUG
            std::cout << "Camera: {" << m_camera.getCameraPos().x << "," << m_camera.getCameraPos().y << ","
                                  << m_camera.getCameraPos().z << "}" << std::endl;
            std::cout << std::endl;
#endif
        }
    }
}
}  // namespace SR
