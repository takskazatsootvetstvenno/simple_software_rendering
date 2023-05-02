#include <cstdint>
#include <array>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
class SDL_Window;
class SDL_Surface;

class Window {
 public:
    enum class WindowEvent {
        DEFAULT = 0,
        KEY_PRESSED_A = 1,
        KEY_PRESSED_D = 2,
        KEY_PRESSED_W,
        KEY_PRESSED_S,
        KEY_PRESSED_Q,
        KEY_PRESSED_E,
        KEY_PRESSED_SPACE
    };
    Window(uint32_t width, uint32_t height);
    void setPixel(const uint32_t x, const uint32_t y, const glm::u8vec4 color) noexcept;
    glm::u8vec4 getPixelColor(const uint32_t x, const uint32_t y) noexcept;
    void clearWindow(glm::u8vec4 color);
    std::array<uint32_t, 2> getExtent() const noexcept;
    void updateScreen() const;
    WindowEvent updateEvents();
    bool windowShouldClose() noexcept;
    ~Window();

 private:
    uint32_t m_width;
    uint32_t m_height;
    SDL_Window* m_window = nullptr;
    SDL_Surface* m_surface = nullptr;
    bool m_shouldClose = false;
};