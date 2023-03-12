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
    SDL_FillRect(m_surface, NULL, SDL_MapRGB(m_surface->format, 0, 255, 0)); 
}
void Window::update() const {
    auto result = SDL_UpdateWindowSurface(m_window);
    if (result != 0) { 
        auto error = std::string("Error during update window surface!") + SDL_GetError();
        throw std::runtime_error(error);
    }
}
Window::~Window() {
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}