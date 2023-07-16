#pragma once
#include <vector>
#include "CommonDefines.hpp"

namespace SR {

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

class Mesh {
 public:
    void setModelMatrix(const glm::mat4& model);
    void setVertexData(std::vector<VertexInput>&& data) noexcept;
    void setIndexData(std::vector<unsigned int>&& indices);
    void setBoundingBox(glm::vec3 minPoint, glm::vec3 maxPoint) noexcept;
    AABB getBoundingBox() const noexcept;
    const std::vector<VertexInput>& getVertexData();
    const std::vector<unsigned int>& getIndicesData();
    const glm::mat4& getModelMatrix() const noexcept;
    Mesh() = default;

 private:
    std::vector<VertexInput> m_vertexData;
    std::vector<unsigned int> m_indices;
    glm::mat4 m_modelMatrix{1.f};
    AABB m_boundingBox{};
};
}  // namespace SR
