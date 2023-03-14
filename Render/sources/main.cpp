#include "Application.hpp"

#include "OBJ_Loader.h"

#include <iostream>
#include <string>
#include <vector>

void setGlobalLocale() {
    try {
        std::locale::global(std::locale("en_US.UTF-8"));
    } catch (const std::exception& e) {
        std::cerr << "Error with setting global locale (en_US.UTF-8)!\n" << e.what() << std::endl;
    }
    std::locale locc;
    std::cout << "Used locale: " << locc.name() << std::endl;
}

void loadMeshesFromOBJ(const std::string& path, SR::Application& app) {
    objl::Loader loader;
    auto result = loader.LoadFile(path);
    if (result) {
        std::cout << "Loading model \"" << path << "\"..." << std::endl;
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
            std::vector<uint32_t> indices;
            for (auto ind : mesh.Indices) { indices.emplace_back(ind); }
            current_mesh.setIndexData(std::move(indices));
            meshes.emplace_back(std::move(current_mesh));
        }
        app.setMeshes(std::move(meshes));
        std::cout << "Loading complete!" << std::endl;
    } else {
        std::cerr << "Can't load model! Path: " << path << std::endl;
    }
}

int main(int argc, char** args) {
    try {
#ifndef NDEBUG
        std::cout << "Render was built with \"Debug\" profile" << std::endl;
#else
        std::cout << "Render was built with \"Release\" profile" << std::endl;
#endif
        setGlobalLocale();
        SR::Application app(1280, 720);
        loadMeshesFromOBJ("../Models/boxTest.obj", app);
        app.start();
    } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }

    return 0;
}
