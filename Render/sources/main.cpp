#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Window.hpp"

int main(int argc, char **args)
{
    try {
        Window myWindow(800, 600);
        myWindow.update();
        system("pause");
    } catch (const std::exception& e) { 
        std::cerr << e.what() << std::endl;
    }

 return 0;
}