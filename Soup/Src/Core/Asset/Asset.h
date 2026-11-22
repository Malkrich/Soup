#pragma once

#include "Core/UUID.h"

namespace Soup
{

  enum class AssetType
  {
    None = 0,

    Mesh,

    Texture2D,
    Shader
  };

  using AssetHandle = UUID;

  class Asset
  {
  public:
    virtual ~Asset() = default;

    AssetHandle Handle = 0;

    virtual AssetType GetAssetType() const = 0;
  };

}
