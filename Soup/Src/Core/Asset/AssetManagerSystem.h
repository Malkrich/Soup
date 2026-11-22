#pragma once

#include "Asset.h"
#include "AssetMetadata.h"
#include "Importer.h"

namespace Soup
{

  class AssetManagerSystem
  {
  public:
    AssetManagerSystem();

    /**
     * Import the asset at the given file path.
     * The asset type is deducted from template type.
     * The asset will have a brand new asset handle assigned.
     */
    template <typename T>
    Ref<Asset> ImportAsset(const std::filesystem::path& filePath) requires(std::is_base_of<Asset, T>::value)
    {
      Ref<Asset> asset = CreateRef<T>();
      ImportAsset(filePath, asset);
      return asset;
    }

    /**
     * Return the asset associated to the given handle.
     * If the asset could not be queried, returns a nullptr.
     */
    Ref<Asset> GetAsset(AssetHandle assetHandle);

    const std::map<AssetHandle, AssetMetadata>& GetAssetRegistry() const { return m_AssetRegistry; }

    /**
     * Add support to read the file for the given extension.
     * Asset type is the expected type when importing the file.
     */
    void AddExtensionSupport(
      AssetType assetType, const std::string& extension, Importer::AssetImportFunction importFunction);

    ///@{
    /**
     * Load/Save the asset registry at the given file path.
     * The asset registry is a json file containing an array of all registered assets with their associated metadata.
     */
    void LoadAssetRegistry(const std::filesystem::path& filePath);
    void SaveAssetRegistry(const std::filesystem::path& filePath) const;
    ///@}

  private:
    /**
     * Import the asset at the given file path.
     * If the import succeeds, the given asset handle will be assigned.
     */
    Ref<Asset> ImportAsset(AssetHandle assetHandle, const AssetMetadata& metadata);

    /**
     * Internal function for importing a new asset from a file path.
     * The asset is written in the asset argument.
     */
    void ImportAsset(const std::filesystem::path& filePath, Ref<Asset>& asset);

    /**
     * Deduct the asset type from the file extension.
     * Browse all available importers and find the first one that can import the given file.
     * Return AssetType::None if no importer matches the file path.
     */
    AssetType DeductAssetType(const std::filesystem::path& filePath);

  private:
    std::map<AssetHandle, Ref<Asset>> m_ImportedAssets;
    std::map<AssetHandle, AssetMetadata> m_AssetRegistry;

    std::map<AssetType, Ref<Importer>> m_Importers;
  };

}
