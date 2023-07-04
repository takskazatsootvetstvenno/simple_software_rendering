#include "Mesh.hpp"
namespace SR {

void Mesh::setModelMatrix(const glm::mat4& model) { m_modelMatrix = model; }

void Mesh::setVertexData(std::vector<vertexInput>&& data) noexcept { m_vertexData = std::move(data); }

void Mesh::setIndexData(std::vector<unsigned int>&& indices) {
    m_indices = std::move(indices);
    m_tempTransformedVertexData.reserve(m_indices.size());
}

void Mesh::setBoundingBox(glm::vec3 minPoint, glm::vec3 maxPoint) noexcept { m_boundingBox = AABB{minPoint, maxPoint}; }

AABB Mesh::getBoundingBox() const noexcept { return m_boundingBox; }

const std::vector<vertexInput>& Mesh::getVertexData() { return m_vertexData; }

const std::vector<unsigned int>& Mesh::getIndicesData() { return m_indices; }

const glm::mat4& Mesh::getModelMatrix() const noexcept { return m_modelMatrix; }

std::vector<VStoFSBuffer>& Mesh::getVertexStageBuffer() const {
    m_tempTransformedVertexData.clear();
    return m_tempTransformedVertexData;
}

}  // namespace SR
