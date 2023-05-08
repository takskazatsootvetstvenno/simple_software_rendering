#include "Application.hpp"

#include "OBJ_Loader.h"

#include <iostream>
#include <string>
#include <vector>
#include <cstring>

void setGlobalLocale() {
    try {
        std::locale::global(std::locale("en_US.UTF-8"));
    } catch (const std::exception& e) {
        std::cerr << "Error with setting global locale (en_US.UTF-8)!\n" << e.what() << std::endl;
    }
    std::locale locc;
    std::cout << "Used locale: " << locc.name() << std::endl;
}

bool loadMeshesFromOBJ(const std::string& path, SR::Application& app) {
    objl::Loader loader;
    auto result = loader.LoadFile(path);
    if (result) {
        std::cout << "\nLoading model \"" << path << "\"..." << std::endl;
        std::vector<SR::Mesh> meshes;
        for (auto& mesh : loader.LoadedMeshes) {
            SR::Mesh current_mesh;
            std::vector<SR::vertexInput> vertexData;
            for (auto& vertex : mesh.Vertices) {
                vertexData.push_back({{vertex.Position.X, vertex.Position.Y, vertex.Position.Z},
                                      {1, 0, 1},
                                      {vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z}});
            }
            current_mesh.setVertexData(std::move(vertexData));
            using render_mesh_index_type = std::remove_reference_t<decltype(current_mesh.getIndicesData())>::value_type;
            static_assert(std::is_same_v<render_mesh_index_type, decltype(mesh.Indices)::value_type>,
                          "Object loader index type and render index type are different!");
            current_mesh.setIndexData(std::move(mesh.Indices));
            current_mesh.setModelMatrix(
                glm::scale(glm::mat4{1.f}, glm::vec3(10.f)));  // Set properly for model! TO DO and auto resize
            meshes.emplace_back(std::move(current_mesh));
        }
        app.setMeshes(std::move(meshes));
        std::cout << "Loading complete!\n" << std::endl;
    } else {
        std::cerr << "Can't load model! Path: " << path << std::endl;
        return false;
    }
    return true;
}
struct ParsedArguments {
    uint32_t width = 1280;
    uint32_t height = 720;
    bool exportToBMP = true;
    bool showHelp = false;
    const char* pathToFile = "../Models/pyramid/pyramid.obj";
};

ParsedArguments parseCLI(const int argc, char** args) noexcept {
    ParsedArguments arguments;
    switch (argc) {
        case 5: arguments.pathToFile = args[4];
        case 4:
            if (std::strncmp(args[3], "N", 2) == 0 || std::strncmp(args[3], "n", 2) == 0) arguments.exportToBMP = false;
        case 3: arguments.width = std::atoi(args[1]); arguments.height = std::atoi(args[2]);
    }
    if (argc == 2 || argc > 5) arguments.showHelp = true;

    return arguments;
}

int main(int argc, char** args) {
    try {
#ifndef NDEBUG
        std::cout << "Render was built with \"Debug\" profile" << std::endl;
#else
        std::cout << "Render was built with \"Release\" profile" << std::endl;
#endif
        setGlobalLocale();
        ParsedArguments arguments = parseCLI(argc, args);
        if (arguments.showHelp) {
            std::cout << "\nWelcome to Simple software render!\n"
                      << "This program can be build with SDL window or with RAM buffer window (Off-screen rendering)\n"
                      << "(Use flag \"FORCE_BUFFER_WINDOW\" to build Off-screen render)\n"
                      << "Also program can export first frame to .BMP image in executable directory\n\nArguments:\n"
                      << " #1 - window width or \"--help\"\n"
                      << " #2 - window height\n"
                      << " #3 - export first frame to BMP file. Can be \"Y\" or \"N\"\n"
                      << " #4 - path to model file (.obj file).\n"
                      << "Examples:\n SoftwareRender 1280 720 Y ../Models/pyramid/pyramid.obj\n"
                      << " SoftwareRender 800 600 N ../Models/boxTest/boxTest.obj" << std::endl;
            return 0;
        }
        SR::Application app(arguments.width, arguments.height, arguments.exportToBMP);
        if (arguments.exportToBMP) std::cout << "First frame will be exported as .BMP file\n";
        bool result = loadMeshesFromOBJ(arguments.pathToFile, app);
        if (!result) return 1;
        app.start();
    } catch (const std::exception& e) {
        std::cerr << "Critic Error: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}
