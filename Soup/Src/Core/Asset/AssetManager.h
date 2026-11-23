#pragma once

#include "Asset.h"
#include "AssetManagerSystem.h"
#include "Project/Project.h"

namespace Soup
{

  class AssetManager
  {
  public:
    /**
     * Import an asset from the given file path to the active asset manager system.
     */
    template <typename T>
    static Ref<T> ImportAsset(const std::filesystem::path& filePath) requires(std::is_base_of<Asset, T>::value)
    {
      Ref<Asset> asset = Project::GetActiveAssetManagerSystem()->ImportAsset<T>(filePath);
      return std::dynamic_pointer_cast<T>(asset);
    }

    /**
     * Return the asset with the given asset handle from the active asset manager system.
     */
    template <typename T>
    static Ref<T> GetAsset(AssetHandle assetHandle) requires(std::is_base_of<Asset, T>::value)
    {
      Ref<Asset> asset = Project::GetActiveAssetManagerSystem()->GetAsset(assetHandle);
      return std::dynamic_pointer_cast<T>(asset);
    }

    /**
     * Add extension support to the active asset manager system.I
     */
    static void AddExtensionSupport(
      AssetType assetType, const std::string& extension, Importer::AssetImportFunction importFunction)
    {
      Project::GetActiveAssetManagerSystem()->AddExtensionSupport(assetType, extension, importFunction);
    }
  };

}
