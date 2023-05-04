#ifndef FORCE_BUFFER_WINDOW
#include <iostream>
#include "SDL.h"
#include "Window.hpp"

extern void exportToBMP(const uint32_t* imageBuffer, const uint32_t width, const uint32_t height) noexcept;
namespace SR {
Window::Window(uint32_t width, uint32_t height)
	:m_width(width), m_height(height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        auto error = std::string("Can't init SDL! Error: ") + SDL_GetError();
        throw std::runtime_error(error);
    }
    m_window = SDL_CreateWindow("Simple software rendering", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,
                                height, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
        auto error = std::string("Can't create SDL window! Error: ") + SDL_GetError();
        throw std::runtime_error(error);
    }
    m_surface = SDL_GetWindowSurface(m_window);
    if (m_surface == nullptr) {
        auto error = std::string("Can't get window surface! Error: ") + SDL_GetError();
        throw std::runtime_error(error);
    }
    std::cout << "Current window pixel format is: " << SDL_GetPixelFormatName(m_surface->format->format) << std::endl;
    std::cout << "Current resolution is: (" << width << " x " << height << ")" << std::endl;
    if (m_surface->format->BitsPerPixel != 32) {
        std::cerr << "Warning! Not a 32-bit per pixel surface!" << std::endl;
    }
}

void Window::setPixel(const uint32_t x, const uint32_t y, const glm::u8vec4 color) noexcept {
    assert(x < m_width && y < m_height && "setPixel: {x, y} outside the screen!");
    uint32_t* buffer = static_cast<uint32_t*>(m_surface->pixels); 
    auto width = m_surface->w;
    buffer[x + y * width] = SDL_MapRGBA(m_surface->format, color.r, color.g, color.b, color.a);
}

glm::u8vec4 Window::getPixelColor(const uint32_t x, const uint32_t y) noexcept {
    const uint32_t* buffer = static_cast<uint32_t*>(m_surface->pixels);
    auto width = m_surface->w;
    glm::u8vec4 pixelColor;
    SDL_GetRGBA(buffer[x + y * width], m_surface->format, &pixelColor.r, &pixelColor.g, &pixelColor.b, &pixelColor.a);
    return pixelColor;
}

void Window::clearWindow(const glm::u8vec4 color) {
    auto result = SDL_FillRect(m_surface, nullptr, SDL_MapRGBA(m_surface->format, color.r, color.g, color.b, color.a));
    if (result != 0) {
        auto error = std::string("Error during clearing surface!") + SDL_GetError();
        throw std::runtime_error(error);
    }
}

std::array<uint32_t, 2> Window::getExtent() const noexcept { return {m_width, m_height}; }

void Window::updateScreen() const {
    auto result = SDL_UpdateWindowSurface(m_window);
    if (result != 0) { 
        auto error = std::string("Error during update window surface!") + SDL_GetError();
        throw std::runtime_error(error);
    }
}

void Window::exportToBMP() const noexcept
{ 
    ::exportToBMP(static_cast<uint32_t*>(m_surface->pixels), m_width, m_height);
}

Window::WindowEvent Window::updateEvents() {
    SDL_Event window_event;
    WindowEvent current_event = WindowEvent::NO_KEYS;
    if (SDL_PollEvent(&window_event) == 1) {
        switch (window_event.type) {
            case SDL_QUIT:
                m_shouldClose = true;
                break;
            case SDL_KEYDOWN:
                switch (window_event.key.keysym.scancode) {
                    case SDL_SCANCODE_W: 
                        current_event = WindowEvent::KEY_PRESSED_W;
                        break;
                    case SDL_SCANCODE_S: 
                         current_event = WindowEvent::KEY_PRESSED_S;
                        break;
                    case SDL_SCANCODE_A: current_event = WindowEvent::KEY_PRESSED_A; break;
                    case SDL_SCANCODE_D: current_event = WindowEvent::KEY_PRESSED_D; break;
                    case SDL_SCANCODE_Q: current_event = WindowEvent::KEY_PRESSED_Q; break;
                    case SDL_SCANCODE_E: current_event = WindowEvent::KEY_PRESSED_E; break;
                    case SDL_SCANCODE_SPACE: current_event = WindowEvent::KEY_PRESSED_SPACE; break;
                    case SDL_SCANCODE_ESCAPE: m_shouldClose = true; break;
                }
                break;
        }
    }
    return current_event;
}
bool Window::windowShouldClose() noexcept {
    return m_shouldClose;
}

Window::~Window() {
    if (m_window != nullptr)
        SDL_DestroyWindow(m_window);
    SDL_Quit();
}
}
#endif
