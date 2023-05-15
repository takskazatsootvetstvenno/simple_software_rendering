#include <cstdint>
#include <array>
#include <vector>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>

#ifndef FORCE_BUFFER_WINDOW
class SDL_Window;
class SDL_Surface;
#endif
namespace SR {

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
        KEY_PRESSED_SPACE,
        NO_KEYS
    };

    struct ClearRect {
        uint32_t min_x;
        uint32_t max_x;
        uint32_t min_y; 
        uint32_t max_y;
    };

    Window(uint32_t width, uint32_t height);
    void setPixel(const uint32_t x, const uint32_t y, const glm::u8vec4 color) noexcept;
    glm::u8vec4 getPixelColor(const uint32_t x, const uint32_t y) noexcept;
    void clearWindow(glm::u8vec4 color) noexcept;
    void setClearRect(ClearRect clearVector) noexcept;
    std::array<uint32_t, 2> getExtent() const noexcept;
    void updateScreen() const;
    WindowEvent updateEvents();
    bool windowShouldClose() noexcept;
    void exportToBMP() const noexcept;
    ~Window();

 private:
    ClearRect m_clearRect{};
    uint32_t m_width;
    uint32_t m_height;
#ifndef FORCE_BUFFER_WINDOW
    SDL_Window* m_window = nullptr;
    SDL_Surface* m_surface = nullptr;
#else
    std::vector<uint32_t> m_buffer;
#endif

    bool m_shouldClose = false;
};

}  // namespace SR