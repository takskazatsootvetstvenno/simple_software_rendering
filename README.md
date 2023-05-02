# Simple software rendering
This is a simple software rendering project

![image](https://user-images.githubusercontent.com/47656611/235807810-fca92fce-a76e-4e3e-9659-fbdbf6207088.png)

### build requirements:
* cmake 3.21 or newer
* compiler with C++20 support (MSVC for windows, Clang/GCC for linux)

### Building:
    git clone --recursive https://github.com/takskazatsootvetstvenno/simple_software_rendering
    cd simple_software_rendering
    For Windows: ./create_project_windows.cmd
    For Linux: ./create_project_linux.sh

##### On linux you can choose build type with script:
    ./create_project_linux.sh Release
    or
    ./create_project_linux.sh Debug
##### On Windows you can choose build type via Visual Studio (.sln file will be in "build" directory)

##### Keyboard controls:
* Q and E - model rotation
* W and S - change camera height

##### Used Libraries:
* SDL2 - https://www.libsdl.org/
* GLM  - https://github.com/g-truc/glm
* OBJ-Loader - https://github.com/Bly7/OBJ-Loader
