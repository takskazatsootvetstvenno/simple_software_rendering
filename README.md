# Simple software rendering
This is a simple software rendering project

### build requirements:
* cmake 3.21 or newer
* compiler with C++17 support (MSVC for windows, Clang/GCC for linux)

### Building:
    git clone --recursive https://github.com/takskazatsootvetstvenno/simple_software_rendering
    cd simple_software_rendering
    For Windows: ./create_project_windows.cmd
    For Linux: ./create_project_linux.sh

##### On linux you can choose build type with script:
    ./create_project_linux.sh Release
    or
    ./create_project_linux.sh Debug
##### on Windows you can choose build type via Visual Studio (.sln file will be in "build" directory)
