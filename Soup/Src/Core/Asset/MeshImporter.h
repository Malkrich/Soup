#pragma once

#include "Importer.h"

namespace Soup
{
  
  class MeshImporter : public Importer
  {
  public:
    MeshImporter();

    /**
     * Load a mesh at the given path.
     * If the import fails, it returns a nullptr.
     */
    Ref<Asset> Import(const std::filesystem::path& filePath) override;

    bool CanImport(const std::filesystem::path& filePath) const override;
    void AddImportFunction(const std::string& extension, Importer::AssetImportFunction function) override;

  private:
    std::unordered_map<std::string, Importer::AssetImportFunction> m_ImportFunctions;
  };

}
