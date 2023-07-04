#include "Application.hpp"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <array>
#include <type_traits>
#include <algorithm>
#include <thread>

static constexpr int NUM_OF_THREADS = 16;
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

template<typename ColorT>
void Application::drawLine(const u32 x1, const u32 y1, const u32 x2, const u32 y2, ColorT color) noexcept {
    u32 max_x = std::max(x1, x2);
    u32 min_x = std::min(x1, x2);
    u32 max_y = std::max(y1, y2);
    u32 min_y = std::min(y1, y2);
    auto pixelColor = color;

    if (max_x - min_x > max_y - min_y) {
        float denominator = static_cast<float>(x1) - x2;
        float k = (static_cast<float>(y1) - y2) / denominator;
        float b = (static_cast<float>(x1) * y2 - x2 * y1) / denominator;
        for (u32 x = min_x; x < max_x; ++x) {
            u32 y = k * x + b;
            m_window.setPixel(x, y, pixelColor);
        }
    } else {
        float denominator = static_cast<float>(y1) - y2;
        float k = (static_cast<float>(x1) - x2) / denominator;
        float b = (static_cast<float>(x1) * y2 - x2 * y1) / denominator;
        for (u32 y = min_y; y < max_y; ++y) {
            u32 x = k * y - b; 
            m_window.setPixel(x, y, pixelColor);
        }
    }
}
template<>
void Application::drawLine(const u32 x1, const u32 y1, const u32 x2, const u32 y2, glm::uvec4 color) noexcept {
    drawLine(x1, y1, x2, y2, m_window.getWindowColorFromVector(color));
}

inline bool vec2CrossSGN(glm::vec2 p1, glm::vec2 p2) noexcept {
    return p1.x * p2.y >= p2.x * p1.y;
}

inline bool isInsideTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 point, bool& sign) noexcept { 
    sign = vec2CrossSGN(point - p1, p1 - p2);
    return (sign == vec2CrossSGN(point - p2, p2 - p3) && sign == vec2CrossSGN(point - p3, p3 - p1));
}

template<typename ColorT>
void Application::drawTriangle(uVec2 p1, uVec2 p2, uVec2 p3, ColorT color) noexcept {
#if 0
    drawLine(p1.x, p1.y, p2.x, p2.y, color);
    drawLine(p2.x, p2.y, p3.x, p3.y, color);
    drawLine(p3.x, p3.y, p1.x, p1.y, color);
#else

    auto boundingBox = findBoundingBox(p1, p2, p3);
    bool sign = false;
    for (u32 y = boundingBox[0].y; y < boundingBox[1].y; ++y) {
        for (u32 x = boundingBox[0].x; x < boundingBox[1].x; ++x) {
            if (isInsideTriangle(p1, p2, p3, {x, y}, sign)) 
            { m_window.setPixel(x, y, color); }
        }
    }
    drawLine(p1.x, p1.y, p2.x, p2.y, ColorT{0});
    drawLine(p2.x, p2.y, p3.x, p3.y, ColorT{0});
    drawLine(p3.x, p3.y, p1.x, p1.y, ColorT{0});
#endif
}

static inline void checkClearBoards(Window::ClearRect& rect, float x, float y) noexcept
{
    if (x < rect.min_x) rect.min_x = static_cast<uint32_t>(x);
    if (x > rect.max_x) rect.max_x = static_cast<uint32_t>(std::ceil(x));
    if (y < rect.min_y) rect.min_y = static_cast<uint32_t>(y);
    if (y > rect.max_y) rect.max_y = static_cast<uint32_t>(std::ceil(y));
}
const auto light_position = glm::normalize(glm::vec3(2.f, 1.f, 4.f));

static inline bool isTriangle(const glm::vec2 A, const glm::vec2 B, const glm::vec2 C) noexcept {
    return (C.x - A.x) * (B.y - A.y) - (B.x - A.x) * (C.y - A.y) <= std::numeric_limits<float>::min();
}

void gpu_thread(const int num_thread, std::vector<VStoFSBuffer>& transformed_vertices, Window& window) noexcept {
    for (int i = 0; i < window.height(); ++i) {
        for (int j = num_thread; j < window.width(); j += NUM_OF_THREADS) {
            float z_buffer = std::numeric_limits<float>::max();
            for (uint32_t k = 0; k < transformed_vertices.size(); k += 3) {
                bool sign = false;
                auto A = glm::vec3(transformed_vertices[k].pos);
                auto B = glm::vec3(transformed_vertices[k + 1].pos);
                auto C = glm::vec3(transformed_vertices[k + 2].pos);
                if (isTriangle(A, B, C) && isInsideTriangle(A, B, C, glm::vec2{j, i}, sign)) {
                    if (sign == false) continue;
                    glm::vec3 AB = glm::vec3(B - A);
                    glm::vec3 AC = glm::vec3(C - A);
                    auto VertNormal = glm::normalize(glm::cross(AB, AC));
                    auto& An = VertNormal.x;
                    auto& Bn = VertNormal.y;
                    auto& Cn = VertNormal.z;

                    const float point_z = A.z - (An * (j - A.x) + Bn * (i - A.y)) / Cn;
                    glm::vec3 P = glm::vec3{j, i, point_z};
                    auto PA_len = glm::length(P - A);
                    auto PB_len = glm::length(P - B);
                    auto PC_len = glm::length(P - C);
                    auto A_weight = glm::length(glm::cross(P - B, P - C));
                    auto B_weight = glm::length(glm::cross(P - A, P - C));
                    auto C_weight = glm::length(glm::cross(P - A, P - B));

                    const auto max_weight = std::max({A_weight, B_weight, C_weight});

                    glm::vec3 normal1 = transformed_vertices[k].normal;
                    glm::vec3 normal2 = transformed_vertices[k + 1].normal;
                    glm::vec3 normal3 = transformed_vertices[k + 2].normal;

                    glm::vec3 normal = glm::normalize((normal1 * (A_weight / max_weight)) + 
                         normal2 * (B_weight / max_weight) + 
                         normal3 * (C_weight / max_weight));
                    glm::vec3 result_color = glm::vec3{245.f} * std::max(glm::dot(normal, light_position), 0.f) +
                                             glm::vec3{10.f, 10.f, 10.f};
                    if (point_z < z_buffer) {
                        z_buffer = point_z;
                        window.setPixel(j, i, window.getWindowColorFromVector(glm::vec4{result_color, 1.f}));
                    }
                }
            }
        }
    }
}

void Application::drawMeshes() { 
    glm::mat4 view = m_camera.getView(); 
    glm::mat4 projection = m_camera.getProjection();
    glm::mat4 screen = m_camera.getNDCtoScreenMatrix();
    Color redColor{255, 0, 0, 255};
    auto windowColor = m_window.getWindowColorFromVector(redColor);
    size_t unclipped_vertices = 0;
    Window::ClearRect clearRect = 
        {m_window.getExtent()[0], 0, m_window.getExtent()[1], 0};

    for (auto& mesh : m_meshes) { 
        auto& transformed_vertices = mesh.getVertexStageBuffer();
        auto& inputVertexMas = mesh.getVertexData();
        glm::mat4 model = mesh.getModelMatrix();
        glm::mat4 MVP = projection * view * model;
        for (auto vertex_index : mesh.getIndicesData())
        {  
            auto& inputPosition = inputVertexMas[vertex_index].position;
            auto& inputNormals = inputVertexMas[vertex_index].normal;
            auto pos = MVP * glm::vec4(inputPosition, 1.f);           

            //Perspective devision
            float const w_reciprocal{1.0f / pos.w};
            pos.x *= w_reciprocal;
            pos.y *= w_reciprocal;
            pos.z *= w_reciprocal;
            pos.w = 1.f;

            //To screen space
            pos = screen * pos;

            transformed_vertices.emplace_back(glm::vec3(pos), inputNormals);
        }

        std::array<std::thread, NUM_OF_THREADS> threads;
        for (int k = 0; k < threads.size(); ++k) 
            threads[k] = std::thread(gpu_thread, k, std::ref(transformed_vertices), std::ref(m_window));
        for (int k = 0; k < threads.size(); ++k) 
            threads[k].join();
    }
    m_window.setClearRect(clearRect);
#ifndef NDEBUG
    size_t all_vertices = 0;
    for (auto& mesh : m_meshes) { all_vertices += mesh.getVertexData().size(); }
    std::cout << "Drawed " << unclipped_vertices << "(" << all_vertices << ")" << std::endl;
#endif  // DEBUG
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
