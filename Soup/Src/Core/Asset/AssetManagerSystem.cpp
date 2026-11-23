#include "AssetManagerSystem.h"

#include "MeshImporter.h"
#include "Project/Project.h"

#include <nlohmann/json.hpp>

namespace Soup
{

  namespace Utils
  {

    static std::string AssetTypeToString(AssetType assetType)
    {
      switch (assetType)
      {
        case AssetType::None:
          return "None";
        case AssetType::Mesh:
          return "Mesh";
        default:
          SP_ASSERT(false, "Unsupported string conversion for asset type {}", (uint32_t)assetType);
          return "";
      }
    }

    AssetType StringToAssetType(const std::string& assetType)
    {
      if (assetType == "Mesh")
      {
        return AssetType::Mesh;
      }
      else
      {
        SP_ASSERT(false, "Unsupported asset type: {}", assetType);
        return AssetType::None;
      }
    }

    std::pair<AssetHandle, AssetMetadata> DeserializeAsset(nlohmann::json assetNode)
    {
      AssetHandle assetHandle = assetNode["AssetHandle"].get<uint64_t>();

      AssetMetadata metadata;
      metadata.Type = StringToAssetType(assetNode["Type"].get<std::string>());
      metadata.FilePath = assetNode["FilePath"].get<std::string>();

      return { assetHandle, metadata };
    }

    nlohmann::json SerializeAsset(AssetHandle assetHandle, const AssetMetadata& metadata)
    {
      nlohmann::json assetNode;
      assetNode["AssetHandle"] = (uint64_t)assetHandle;
      assetNode["Type"] = AssetTypeToString(metadata.Type);
      assetNode["FilePath"] = metadata.FilePath.generic_string();

      return assetNode;
    }
  }

  AssetManagerSystem::AssetManagerSystem()
  {
    m_Importers[AssetType::Mesh] = CreateRef<MeshImporter>();
  }

  Ref<Asset> AssetManagerSystem::GetAsset(AssetHandle assetHandle)
  {
    auto importedAssetIt = m_ImportedAssets.find(assetHandle);
    if (importedAssetIt != m_ImportedAssets.end())
    {
      return importedAssetIt->second;
    }

    auto registeredAssetIt = m_AssetRegistry.find(assetHandle);
    if (registeredAssetIt != m_AssetRegistry.end())
    {
      return ImportAsset(registeredAssetIt->first, registeredAssetIt->second);
    }

    return nullptr;
  }

  void AssetManagerSystem::AddExtensionSupport(
    AssetType assetType, const std::string& extension, Importer::AssetImportFunction importFunction)
  {
    auto it = m_Importers.find(assetType);
    SP_ASSERT(it != m_Importers.end(), "Importer not found for asset type {}", Utils::AssetTypeToString(assetType));

    it->second->AddImportFunction(extension, importFunction);
  }

  void AssetManagerSystem::LoadAssetRegistry(const std::filesystem::path& filePath)
  {
    SP_ASSERT(std::filesystem::exists(filePath), "No asset registry found at {}", filePath.generic_string());
    std::ifstream registryFile(filePath);
    nlohmann::json jsonRoot = nlohmann::json::parse(registryFile);

    SP_ASSERT(jsonRoot.find("Assets") != jsonRoot.end(), "Could not find assets array in assets registry!");
    nlohmann::json assetNodes = jsonRoot["Assets"];
    for (const auto& assetNode : assetNodes)
    {
      auto [handle, metadata] = Utils::DeserializeAsset(assetNode);
      m_AssetRegistry[handle] = metadata;
    }
  }

  void AssetManagerSystem::SaveAssetRegistry(const std::filesystem::path& filePath) const
  {
    nlohmann::json jsonRoot;

    nlohmann::json& assetNodes = jsonRoot["Assets"];
    for (const auto& [assetHandle, metadata] : m_AssetRegistry)
    {
      nlohmann::json assetNode = Utils::SerializeAsset(assetHandle, metadata);
      assetNodes.push_back(assetNode);
    }

    std::ofstream jsonFile(filePath);
    jsonFile << jsonRoot.dump(4) + '\n';
  }

  Ref<Asset> AssetManagerSystem::ImportAsset(AssetHandle assetHandle, const AssetMetadata& metadata)
  {
    auto importerIt = m_Importers.find(metadata.Type);
    if (importerIt == m_Importers.end())
    {
      SP_LOG_ERROR("Could not find importer for asset type {}", Utils::AssetTypeToString(metadata.Type));
      return nullptr;
    }

    if (!importerIt->second->CanImport(metadata.FilePath))
    {
      SP_LOG_ERROR("File format not supported for file {} with asset type {}", metadata.FilePath.generic_string(),
        Utils::AssetTypeToString(metadata.Type));
      return nullptr;
    }

    std::filesystem::path fullPath = Project::GetActiveWorkingDirectory() / metadata.FilePath;
    Ref<Asset> asset = importerIt->second->Import(fullPath);
    asset->Handle = assetHandle;

    m_ImportedAssets[assetHandle] = asset;

    return asset;
  }

  void AssetManagerSystem::ImportAsset(const std::filesystem::path& filePath, Ref<Asset>& asset)
  {
    std::filesystem::path relativePath = std::filesystem::relative(filePath, Project::GetActiveWorkingDirectory());
    if (relativePath.empty())
    {
      SP_LOG_ERROR("File path {} is not a child of current project active directory ({})", filePath.generic_string(),
        Project::GetActiveWorkingDirectory().generic_string());
      asset = nullptr;
      return;
    }

    if (!std::filesystem::exists(filePath))
    {
      SP_LOG_ERROR("Asset path {} does not exist!", filePath.generic_string());
      asset = nullptr;
      return;
    }

    AssetMetadata metadata;
    metadata.Type = DeductAssetType(filePath);
    metadata.FilePath = relativePath;

    AssetHandle assetHandle;
    m_AssetRegistry[assetHandle] = metadata;

    asset = ImportAsset(assetHandle, metadata);
  }

  AssetType AssetManagerSystem::DeductAssetType(const std::filesystem::path& filePath)
  {
    for (const auto& [assetType, importer] : m_Importers)
    {
      if (importer->CanImport(filePath))
      {
        return assetType;
      }
    }
    return AssetType::None;
  }

}
