#pragma once

#include <glm/glm.hpp>

namespace Soup
{

  class World
  {
  public:
    static constexpr glm::vec3 GetXUnitVector() { return { 1.0f, 0.0f, 0.0f }; }
    static constexpr glm::vec3 GetYUnitVector() { return { 0.0f, 1.0f, 0.0f }; }
    static constexpr glm::vec3 GetZUnitVector() { return { 0.0f, 0.0f, 1.0f }; }

    static constexpr glm::vec3 GetUpVector() { return GetYUnitVector(); }
  };

}
