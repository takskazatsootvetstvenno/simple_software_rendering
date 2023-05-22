#ifdef FORCE_BUFFER_WINDOW

#include "Window.hpp"
#include <iostream>

extern void exportToBMP(const uint32_t* imageBuffer, const uint32_t width, const uint32_t height) noexcept;
extern void exportToBMP(const uint16_t* imageBuffer, const uint32_t width, const uint32_t height) noexcept;

namespace SR {

// Alpha channel will be ignored
buffer_type Window::getWindowColorFromVector(const glm::u8vec4 color) const noexcept {
    buffer_type r = color[0];
    buffer_type g = color[1];
    buffer_type b = color[2];
    if constexpr (std::is_same_v<buffer_type, uint32_t>) { //888
        const buffer_type resultColor = (r << 16) + (g << 8) + b;
        return resultColor;
    } else if constexpr (std::is_same_v<buffer_type, uint16_t>) {  // 565
        const buffer_type resultColor = (r / 8) << 11 | (g / 4) << 5 | (b / 8);
        return resultColor;
    }
}
Window::ClearRect Window::getOverralClearRect() const noexcept {
    ClearRect rect;
    rect.min_x = std::min(m_clearRect.min_x, m_lastClearRect.min_x);
    rect.max_x = std::max(m_clearRect.max_x, m_lastClearRect.max_x);
    rect.min_y = std::min(m_clearRect.min_y, m_lastClearRect.min_y);
    rect.max_y = std::max(m_clearRect.max_y, m_lastClearRect.max_y);
    return rect;
}

void Window::setClearRect(const ClearRect clearVector) noexcept {
    assert(clearVector.min_x >= 0 && "Min x should be greater than 0");
    assert(clearVector.max_x <= m_width && "Max x should be less than screen width");
    assert(clearVector.min_y >= 0 && "Min y should be greater than 0");
    assert(clearVector.max_y <= m_height && "Max y should be less than height");
    m_lastClearRect = m_clearRect;
    m_clearRect = clearVector;
}

// Alpha channel will be ignored
constexpr glm::u8vec4 getVectorFromColor(const uint32_t bufferValue) noexcept {
    uint8_t red = bufferValue & (255 << 16);
    uint8_t green = bufferValue & (255 << 8);
    uint8_t blue = bufferValue & 255;
    return {red, green, blue, 255};
}

// Alpha channel will be ignored
constexpr glm::u8vec4 getVectorFromColor(const uint16_t bufferValue) noexcept {
    uint8_t red = (bufferValue & (31 << 11)) * 8;
    uint8_t green = (bufferValue & (63 << 5)) * 4;
    uint8_t blue = (bufferValue & 31) * 8;
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

void Window::setPixel(const uint32_t x, const uint32_t y, const uint16_t color) noexcept {
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
    using buffer_type = std::remove_reference_t<decltype(m_buffer)>::value_type;
    ::exportToBMP(static_cast<const buffer_type*>(m_buffer.data()), m_width, m_height);
}

Window::WindowEvent Window::updateEvents() {
    // not implemented yet
    return WindowEvent::NO_KEYS;
}
bool Window::windowShouldClose() noexcept { return m_shouldClose; }

Window::~Window() {}
}  // namespace SR
#endif  // BUFFER_WINDOW
