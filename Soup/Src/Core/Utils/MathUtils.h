#pragma once

#include <glm/glm.hpp>

namespace Soup::MathUtils
{

  glm::vec3 SphericalToCartesian(float pitch, float yaw, float distance);

  glm::mat3 ComputeNormalTransform(const glm::mat4& transform);

}
