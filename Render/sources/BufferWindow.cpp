#ifdef FORCE_BUFFER_WINDOW

#include "Window.hpp"
#include <iostream>

extern void exportToBMP(const uint32_t* imageBuffer, const uint32_t width, const uint32_t height) noexcept;

namespace SR {

// Alpha channel will be ignored
uint32_t Window::getWindowColorFromVector(const glm::u8vec4 color) const noexcept {
    const uint32_t resultColor = (color[0] << 16) + (color[1] << 8) + color[2];
    return resultColor;
}

void Window::setClearRect(const ClearRect clearVector) noexcept {
    assert(clearVector.min_x >= 0 && "Min x should be greater than 0");
    assert(clearVector.max_x <= m_width && "Max x should be less than screen width");
    assert(clearVector.min_y >= 0 && "Min y should be greater than 0");
    assert(clearVector.max_y <= m_height && "Max y should be less than height");
    m_clearRect = clearVector;
}

// Alpha channel will be ignored
constexpr glm::u8vec4 getVectorFromColor(const uint32_t bufferValue) noexcept {
    uint8_t red = bufferValue & (255 << 16);
    uint8_t green = bufferValue & (255 << 8);
    uint8_t blue = bufferValue & 255;
    return {red, green, blue, 255};
}

Window::Window(uint32_t width, uint32_t height)
    : m_width(width), m_height(height), m_buffer(width * height), m_clearRect{width, 0, height, 0} {
    std::cout << "Compiled with buffer window (off-screen rendering)!\nFormat will be: RGB888" << std::endl;
    std::cout << "Current resolution is: (" << m_width << " x " << m_height << ")" << std::endl;
}

void Window::setPixel(const uint32_t x, const uint32_t y, const glm::u8vec4 color) noexcept {
    assert(x < m_width && y < m_height && "setPixel: {x, y} outside the screen!");
    m_buffer[x + y * m_width] = getWindowColorFromVector(color);
}

void Window::setPixel(const uint32_t x, const uint32_t y, const uint32_t color) noexcept {
    assert(x < m_width && y < m_height && "setPixel: {x, y} outside the screen!");
    m_buffer[x + y * m_width] = color;
}

glm::u8vec4 Window::getPixelColor(const uint32_t x, const uint32_t y) noexcept {
    return getVectorFromColor(m_buffer[x + y * m_width]);
}

void Window::clearWindow(const glm::u8vec4 color) noexcept {
    const auto bufferColor = getWindowColorFromVector(color);
    for (uint32_t y = m_clearRect.min_y; y < m_clearRect.max_y; ++y)
        for (uint32_t x = m_clearRect.min_x; x < m_clearRect.max_x; ++x) 
            m_buffer[y * m_width + x] = bufferColor;
}

std::array<uint32_t, 2> Window::getExtent() const noexcept { return {m_width, m_height}; }

void Window::updateScreen() const {
    // off-screen rendering, we do not need any sync here
}
void Window::exportToBMP() const noexcept {
    ::exportToBMP(static_cast<const uint32_t*>(m_buffer.data()), m_width, m_height);
}

Window::WindowEvent Window::updateEvents() {
    // not implemented yet
    return WindowEvent::NO_KEYS;
}
bool Window::windowShouldClose() noexcept { return m_shouldClose; }

Window::~Window() {}
}  // namespace SR
#endif  // BUFFER_WINDOW
