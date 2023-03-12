#include <cstdint>
#include <glm/glm.hpp>
class SDL_Window;
class SDL_Surface;

class Window {
 public:
    Window(unsigned int width, unsigned int height);
    void setPixel(const uint32_t x, const uint32_t y, const glm::u8vec4 color) noexcept;
    glm::u8vec4 getPixelColor(const uint32_t x, const uint32_t y) noexcept;
    void clearWindow(glm::u8vec4 color);
    void update() const;
    bool windowShouldClose() noexcept;
    ~Window();
 private:
    unsigned int m_width;
    unsigned int m_height;
    SDL_Window* m_window = nullptr;
    SDL_Surface* m_surface = nullptr;
    bool m_shouldClose = false;
};