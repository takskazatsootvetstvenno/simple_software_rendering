//
class SDL_Window;
class SDL_Surface;

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0; 
    uint8_t a = 0;
};

class Window {
 public:
    Window(unsigned int width, unsigned int height);
    void setPixel(const uint32_t x, const uint32_t y, const Color color) noexcept;
    Color getPixelColor(const uint32_t x, const uint32_t y) noexcept;
    void clearWindow(const Color color);
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