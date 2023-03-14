#include <cstdint>
#include <glm/glm.hpp>
#include <array>
class SDL_Window;
class SDL_Surface;

class Window {
 public:
    Window(uint32_t width, uint32_t height);
    void setPixel(const uint32_t x, const uint32_t y, const glm::u8vec4 color) noexcept;
    glm::u8vec4 getPixelColor(const uint32_t x, const uint32_t y) noexcept;
    void clearWindow(glm::u8vec4 color);
    std::array<uint32_t, 2> getExtent() const noexcept;
    void update() const;
    bool windowShouldClose() noexcept;
    ~Window();

 private:
    uint32_t m_width;
    uint32_t m_height;
    SDL_Window* m_window = nullptr;
    SDL_Surface* m_surface = nullptr;
    bool m_shouldClose = false;
};