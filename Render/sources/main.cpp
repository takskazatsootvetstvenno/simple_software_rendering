#include <iostream>
#include "Window.hpp"

void testDraw(Window& myWindow) {
    myWindow.clearWindow({255, 0, 255, 0});
    myWindow.setPixel(100, 100, {255, 255, 0, 0});
    myWindow.setPixel(200, 100, {255, 0, 0, 0});
    myWindow.setPixel(300, 100, {0, 0, 255, 0});
}

int main(int argc, char **args)
{
    try {
#ifndef NDEBUG
        std::cout << "Render was built with \"Debug\" profile" << std::endl;
#else
        std::cout << "Render was built with \"Release\" profile" << std::endl;
#endif
        Window myWindow(800, 600);
       
        while (!myWindow.windowShouldClose()) {
            testDraw(myWindow);
            myWindow.update();
        }
    } catch (const std::exception& e) { 
        std::cerr << e.what() << std::endl;
    }

 return 0;
}
