#include "Window.hpp"

#include <cstdint>

namespace SR {
class Application {
 public:
    Application(uint32_t width, uint32_t height);
    ~Application();
    void start();
    void drawLoop();

 private:
    Window m_window;
};
}  // namespace SR
