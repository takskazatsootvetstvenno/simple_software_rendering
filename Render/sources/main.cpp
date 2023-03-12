#include <iostream>
#include "Window.hpp"

int main(int argc, char **args)
{
    try {
#ifndef NDEBUG
        std::cout << "Render was built with debug profile" << std::endl;
#else
        std::cout << "Render was built with release profile" << std::endl;
#endif
        Window myWindow(800, 600);
        myWindow.update();
        auto _ = getchar();
    } catch (const std::exception& e) { 
        std::cerr << e.what() << std::endl;
    }

 return 0;
}
