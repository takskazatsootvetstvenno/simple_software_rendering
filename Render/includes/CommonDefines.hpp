#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>

namespace SR {
struct VertexInput {
    glm::vec<3, float, glm::packed> position{0.f};
    glm::vec<3, float, glm::packed> normal{0.f};
};
}  // namespace SR