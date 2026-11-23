#pragma once

#include "Asset.h"

namespace Soup
{

  struct AssetMetadata
  {
    AssetType Type = AssetType::None;
    std::filesystem::path FilePath;
  };

}
