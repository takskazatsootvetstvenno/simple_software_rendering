#include "Application.hpp"

#include "OBJ_Loader.h"

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <filesystem>

std::pair<SR::Mesh, float> parseMeshes(objl::Mesh& mesh) {
    SR::Mesh current_mesh;
    std::vector<SR::vertexInput> vertexData;
    glm::vec3 min = glm::vec3(std::numeric_limits<float>().max());
    glm::vec3 max = glm::vec3(0);
    for (auto& vertex : mesh.Vertices) {
        vertexData.push_back({{vertex.Position.X, vertex.Position.Y, vertex.Position.Z},
                              {1, 0, 1},
                              {vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z}});
        max.x = std::max(max.x, vertex.Position.X);
        max.y = std::max(max.y, vertex.Position.Y);
        max.z = std::max(max.z, vertex.Position.Z);
        min.x = std::min(min.x, vertex.Position.X);
        min.y = std::min(min.y, vertex.Position.Y);
        min.z = std::min(min.z, vertex.Position.Z);
    }
    current_mesh.setBoundingBox(min, max);
    current_mesh.setVertexData(std::move(vertexData));
    using render_mesh_index_type = std::remove_reference_t<decltype(current_mesh.getIndicesData())>::value_type;
    static_assert(std::is_same_v<render_mesh_index_type, decltype(mesh.Indices)::value_type>,
                  "Object loader index type and render index type are different!");
    current_mesh.setIndexData(std::move(mesh.Indices));

    static float d = 1;
    glm::vec3 delta = (max - min);
    float result_max_distance = std::max(std::max(delta.x, delta.y), delta.z);
    
    return {current_mesh, result_max_distance};
}

bool loadMeshesFromOBJ(const std::string& path, SR::Application& app) {
    objl::Loader loader;
    auto result = loader.LoadFile(path);
    if (result) {
        std::cout << "\nLoading model \"" << path << "\"..." << std::endl;
        auto scalePath = std::filesystem::path(path).parent_path() / "scale.txt";
        float scaleFromFile = 0.f;
        if (std::filesystem::exists(scalePath)) {
            std::cout << "Found scale.txt in model path" << std::endl;
            std::ifstream stream(scalePath);
            stream >> scaleFromFile;
        }
        std::vector<SR::Mesh> meshes;
        float globalMaxDistanceBB = 1.f;
        for (auto& mesh : loader.LoadedMeshes) {
            auto&& [filledMesh, MaxDistance] = parseMeshes(mesh);
            globalMaxDistanceBB = std::max(globalMaxDistanceBB, MaxDistance);
            meshes.emplace_back(std::move(filledMesh)); 
        }
        float globalScale = 1.f / (globalMaxDistanceBB * 0.1);

        if (scaleFromFile > std::numeric_limits<float>().min())
            globalScale = scaleFromFile;

        for (auto& mesh : meshes) 
            mesh.setModelMatrix(glm::scale(glm::mat4{1.f}, glm::vec3(globalScale)));

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
