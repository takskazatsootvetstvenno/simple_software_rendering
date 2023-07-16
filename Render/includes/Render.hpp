#pragma once
#include "Window.hpp"
#include "CommonDefines.hpp"
#include <vector>

namespace SR {
class Render {
    struct VStoFSBuffer {
        glm::vec<3, float, glm::packed> pos;
        glm::vec<3, float, glm::packed> normal;
    };

 public:
    Render(Window& window);

    void setVertexData(const std::vector<VertexInput>& vertexInfo) noexcept;
    void setIndexData(const std::vector<uint32_t>& indexInfo) noexcept;
    void setMVPmatrix(glm::mat4&& MVP) noexcept;
    void setNDCtoScreenMatrix(glm::mat4 screen);

    void draw();
    void setExportBlobs(bool exportBlobs) noexcept;

 private:
    void exportInputData();
    void exportVStoFSBuffer();

    void processVertexShader();
    void processFragmentShader();

    void gpu_thread(const int num_thread) noexcept;

    bool m_exportAllBlobs = true;
    Window& m_window;
    const glm::vec3 m_light_position = glm::normalize(glm::vec3(2.f, 1.f, 4.f));
    const std::vector<VertexInput>* m_vertexInfo = nullptr;
    const std::vector<uint32_t>* m_indexInfo = nullptr;
    std::vector<VStoFSBuffer> m_VStoFSBuffer;
    glm::mat4 m_MVP{};
    glm::mat4 m_NDCtoScreen{};
};

}  // namespace SR