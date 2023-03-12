#include "Application.hpp"

#include <iostream>

int main(int argc, char** args) {
    try {
#ifndef NDEBUG
        std::cout << "Render was built with \"Debug\" profile" << std::endl;
#else
        std::cout << "Render was built with \"Release\" profile" << std::endl;
#endif
        SR::Application app(800, 600);
        app.start();
    } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }

    return 0;
}
