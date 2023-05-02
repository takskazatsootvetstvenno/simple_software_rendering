#include <vector>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>

namespace SR {

struct vertexInput {
    glm::vec3 position{0.f};
    glm::vec3 color{0.f};
    glm::vec3 normal{0.f};
};

class Mesh {
 public:
    void setModelMatrix(const glm::mat4& model);
    void setVertexData(std::vector<vertexInput>&& data) noexcept;
    void setIndexData(std::vector<uint32_t>&& indices) noexcept;
    const std::vector<vertexInput>& getVertexData();
    const std::vector<uint32_t>& getIndicesData();
    const glm::mat4& getModelMatrix() const noexcept;
    Mesh() = default;

 private:
    std::vector<vertexInput> m_vertexData;
    std::vector<uint32_t> m_indices;
    glm::mat4 m_modelMatrix{1.f};
};
}