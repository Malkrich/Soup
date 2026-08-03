#pragma once

namespace Soup::TextureUtils
{

  uint32_t ComputeWidthOrHeightFromMipLevel(uint32_t mipLevel, uint32_t baseWidthOrHeight);

  uint32_t ComputeMaxLevelsFromDimensions(uint32_t dimensions);

}
