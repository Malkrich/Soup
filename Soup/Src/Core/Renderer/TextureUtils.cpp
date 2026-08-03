#include "TextureUtils.h"

#include <glm/glm.hpp>

namespace Soup::TextureUtils
{

  uint32_t ComputeWidthOrHeightFromMipLevel(uint32_t mipLevel, uint32_t baseWidthOrHeight)
  {
    return baseWidthOrHeight * glm::pow(0.5, mipLevel);
  }

  uint32_t ComputeMaxLevelsFromDimensions(uint32_t dimensions)
  {
    return (uint32_t)glm::log2((float)dimensions) + 1;
  }

}
