#include "Render.hpp"
#include <algorithm>
#include <thread>
#include <fstream>

static constexpr int NUM_OF_THREADS = 16;

namespace SR {

// Check if 3 point is a triangle
static inline bool isTriangle(const glm::vec2 A, const glm::vec2 B, const glm::vec2 C) noexcept {
    return (C.x - A.x) * (B.y - A.y) - (B.x - A.x) * (C.y - A.y) <= std::numeric_limits<float>::min();
}

inline bool vec2CrossSGN(glm::vec2 p1, glm::vec2 p2) noexcept { return p1.x * p2.y >= p2.x * p1.y; }

// Check if projection of point is inside a triangle
inline bool isInsideTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 point, bool& sign) noexcept {
    sign = vec2CrossSGN(point - p1, p1 - p2);
    return (sign == vec2CrossSGN(point - p2, p2 - p3) && sign == vec2CrossSGN(point - p3, p3 - p1));
}

void Render::exportInputData() { 
    std::ofstream vertexFile("Vertices.bin", std::ios::binary | std::ios::out);
    std::ofstream indexFile("Indices.bin", std::ios::binary | std::ios::out);
    std::ofstream MVPFile("MVP.bin", std::ios::binary | std::ios::out);
    std::ofstream ScreenMatrixFile("ScreenMatrix.bin", std::ios::binary | std::ios::out);
    
    const auto& vertices = *m_vertexInfo;
    const auto& indices = *m_indexInfo;

    vertexFile.write(reinterpret_cast<const char*>(vertices.data()), vertices.size() * sizeof(VertexInput));
    indexFile.write(reinterpret_cast<const char*>(indices.data()), indices.size() * sizeof(uint32_t));
    MVPFile.write(reinterpret_cast<const char*>(&(m_MVP[0].r)), 16 * sizeof(float));
    ScreenMatrixFile.write(reinterpret_cast<const char*>(&(m_NDCtoScreen[0].r)), 16 * sizeof(float));
}

void Render::exportVStoFSBuffer() { 
    std::ofstream VStoFSBufferFile("VStoFSBuffer.bin", std::ios::binary | std::ios::out); 
    VStoFSBufferFile.write(
        reinterpret_cast<const char*>(m_VStoFSBuffer.data()), m_VStoFSBuffer.size() * sizeof(VStoFSBuffer));
}

void Render::gpu_thread(const int num_thread) noexcept {
    for (unsigned int i = 0; i < m_window.height(); ++i) {
        for (unsigned int j = num_thread; j < m_window.width(); j += NUM_OF_THREADS) {
            float z_buffer = std::numeric_limits<float>::max();

            //iterate per points
            for (uint32_t k = 0; k < m_VStoFSBuffer.size(); k += 3) {
                bool sign = false;
                auto A = glm::vec3(m_VStoFSBuffer[k].pos);
                auto B = glm::vec3(m_VStoFSBuffer[k + 1].pos);
                auto C = glm::vec3(m_VStoFSBuffer[k + 2].pos);

                if (isTriangle(A, B, C) && isInsideTriangle(A, B, C, glm::vec2{j, i}, sign)) {
                    if (sign == false) continue; //Back-face culling

                    glm::vec3 AB = glm::vec3(B - A);
                    glm::vec3 AC = glm::vec3(C - A);
                    auto VertNormal = glm::normalize(glm::cross(AB, AC));
                    auto& An = VertNormal.x;
                    auto& Bn = VertNormal.y;
                    auto& Cn = VertNormal.z;

                    const float point_z = A.z - (An * (j - A.x) + Bn * (i - A.y)) / Cn;

                    //Point P is inside triangle
                    glm::vec3 P = glm::vec3{j, i, point_z};

                    //Length from P to triangle verices
                    auto PA_len = glm::length(P - A);
                    auto PB_len = glm::length(P - B);
                    auto PC_len = glm::length(P - C);

                    auto A_weight = glm::length(glm::cross(P - B, P - C));
                    auto B_weight = glm::length(glm::cross(P - A, P - C));
                    auto C_weight = glm::length(glm::cross(P - A, P - B));

                    const auto max_weight = std::max({A_weight, B_weight, C_weight});

                    //Calculate normal for point P
                    glm::vec3 normal1 = m_VStoFSBuffer[k].normal;
                    glm::vec3 normal2 = m_VStoFSBuffer[k + 1].normal;
                    glm::vec3 normal3 = m_VStoFSBuffer[k + 2].normal;

                    glm::vec3 normal = glm::normalize((normal1 * (A_weight / max_weight)) +
                                                      normal2 * (B_weight / max_weight) +
                                                      normal3 * (C_weight / max_weight));

                    glm::vec3 result_color = glm::vec3{240.f} * std::max(glm::dot(normal, m_light_position), 0.f) +
                                             glm::vec3{15.f, 15.f, 15.f};

                    if (point_z < z_buffer) { //Depth test
                        z_buffer = point_z;
                        m_window.setPixel(j, i, m_window.getWindowColorFromVector(glm::vec4{result_color, 1.f}));
                    }
                }
            }
        }
    }
}
Render::Render(Window& window) : m_window(window) {
    static_assert(sizeof(VertexInput) == sizeof(float) * 6, "Invalid input buffer size");
    static_assert(sizeof(VStoFSBuffer) == sizeof(float) * 6, "Invalid intermediate buffer size");
}

void Render::setVertexData(const std::vector<VertexInput>& vertexInfo) noexcept { m_vertexInfo = &vertexInfo; }

void Render::setIndexData(const std::vector<uint32_t>& indexInfo) noexcept { m_indexInfo = &indexInfo; }

void Render::setMVPmatrix(glm::mat4&& MVP) noexcept { m_MVP = std::move(MVP); }

void Render::setNDCtoScreenMatrix(glm::mat4 screen) { m_NDCtoScreen = std::move(screen); }

void Render::draw() { 
    if (m_exportAllBlobs)
        exportInputData();

    processVertexShader();

    if (m_exportAllBlobs)
        exportVStoFSBuffer();

    processFragmentShader();
}

void Render::setExportBlobs(bool exportBlobs) noexcept { m_exportAllBlobs = exportBlobs; }

void Render::processVertexShader() {
    m_VStoFSBuffer.clear();
    const auto& vertices = *m_vertexInfo;
    const auto& indices = *m_indexInfo;

    for (auto vertex_index : indices) {
        // Start User-defined VS
        auto& inputPosition = vertices[vertex_index].position;
        auto& inputNormals = vertices[vertex_index].normal;
        auto pos = m_MVP * glm::vec4(inputPosition, 1.f);
        // End user-defined VS

        // Perspective devision
        float const w_reciprocal{1.0f / pos.w};
        pos.x *= w_reciprocal;
        pos.y *= w_reciprocal;
        pos.z *= w_reciprocal;
        pos.w = 1.f;

        // To screen space
        pos = m_NDCtoScreen * pos;

        m_VStoFSBuffer.emplace_back(glm::vec3(pos), inputNormals);
    }
}
void Render::processFragmentShader() {
    std::array<std::thread, NUM_OF_THREADS> threads;
    for (int k = 0; k < threads.size(); ++k) 
        threads[k] = std::thread([this, k] { gpu_thread(k); });
    for (int k = 0; k < threads.size(); ++k) threads[k].join();

}
}  // namespace SR
