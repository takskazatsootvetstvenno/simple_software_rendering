#include <iostream>
#include "SDL.h"
#include "Window.hpp"

Window::Window(unsigned int width, unsigned int height)
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
    if (m_surface->format->BitsPerPixel != 32) {
        std::cerr << "Warning! Not a 32-bit per pixel surface!" << std::endl;
    }
}

void Window::setPixel(const uint32_t x, const uint32_t y, const glm::u8vec4 color) noexcept {
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

void Window::update() const {
    auto result = SDL_UpdateWindowSurface(m_window);
    if (result != 0) { 
        auto error = std::string("Error during update window surface!") + SDL_GetError();
        throw std::runtime_error(error);
    }
}

bool Window::windowShouldClose() noexcept {
    SDL_Event window_event;
    if (SDL_PollEvent(&window_event) == 1) {
        if (window_event.type == SDL_QUIT) m_shouldClose = true;
    }
    return m_shouldClose;
}

Window::~Window() {
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}