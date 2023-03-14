#include "Mesh.hpp"
namespace SR {

void Mesh::setModelMatrix(const glm::mat4& model) { m_modelMatrix = model; }

void Mesh::setVertexData(std::vector<vertexInput>&& data) noexcept { m_vertexData = std::move(data); }

void Mesh::setIndexData(std::vector<uint32_t>&& indices) noexcept { m_indices = std::move(indices); }

const std::vector<vertexInput>& Mesh::getVertexData() { return m_vertexData; }

const std::vector<uint32_t>& Mesh::getIndicesData() { return m_indices; }

const glm::mat4& Mesh::getModelMatrix() const noexcept { return m_modelMatrix; }

}  // namespace SR
