#ifdef FORCE_BUFFER_WINDOW

#include "Window.hpp"
#include <iostream>

constexpr uint32_t getColorFromVector(const glm::u8vec4 color) noexcept {
    const uint32_t resultColor = (color[0] << 24) + (color[1] << 16) + (color[2] << 8) + color[3];
    return resultColor;
}

constexpr glm::u8vec4 getVectorFromColor(const uint32_t bufferValue) noexcept {
    uint8_t red = bufferValue & (255 << 24);
    uint8_t green = bufferValue & (255 << 16);
    uint8_t blue = bufferValue & (255 << 8);
    uint8_t alpha = bufferValue & 255;
    return {red, green, blue, alpha};
}

Window::Window(uint32_t width, uint32_t height) : m_width(width), m_height(height), m_buffer(width * height) {
    std::cout << "Compiled with buffer window (off-screen rendering)!\nFormat will be: RGB888" << std::endl;
    std::cout << "Current resolution is: (" << m_width << " x " << m_height << ")\n" << std::endl;
}

void Window::setPixel(const uint32_t x, const uint32_t y, const glm::u8vec4 color) noexcept {
    assert(x < m_width && y < m_height && "setPixel: {x, y} outside the screen!");
    m_buffer[x + y * m_width] = getColorFromVector(color);
}

glm::u8vec4 Window::getPixelColor(const uint32_t x, const uint32_t y) noexcept {   
    return getVectorFromColor(m_buffer[x + y * m_width]);
}

void Window::clearWindow(const glm::u8vec4 color) {
    auto bufferColor = getColorFromVector(color);
    for (uint32_t y = 0; y < m_height; ++y)
        for (uint32_t x = 0; x < m_width; ++x)
            m_buffer[x + y * m_width] = bufferColor;
}

std::array<uint32_t, 2> Window::getExtent() const noexcept { return {m_width, m_height}; }

void Window::updateScreen() const {
    //off-screen rendering, we do not need any sync here
}

Window::WindowEvent Window::updateEvents() {
    //not implemented yet
    return WindowEvent::NO_KEYS;
}
bool Window::windowShouldClose() noexcept { return m_shouldClose; }

Window::~Window() {}

#endif  // BUFFER_WINDOW