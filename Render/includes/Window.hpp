//
class SDL_Window;
class SDL_Surface;

class Window {
 public:
    Window(unsigned int width, unsigned int height);
    void update() const;
    ~Window();
 private:
    unsigned int m_width;
    unsigned int m_height;
    SDL_Window* m_window = nullptr;
    SDL_Surface* m_surface = nullptr;
};