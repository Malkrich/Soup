#pragma once

#include "Asset.h"

namespace Soup
{

  class Importer
  {
  public:
    typedef std::function<Ref<Asset>(const std::filesystem::path&)> AssetImportFunction;

  public:
    virtual ~Importer() = default;

    /**
     * Import the asset at the given path.
     * Return nullptr if the import fails.
     */
    virtual Ref<Asset> Import(const std::filesystem::path& filePath) = 0;

    /**
     * Return true if the file at the given file path can be loaded by this importer.
     */
    virtual bool CanImport(const std::filesystem::path& filePath) const = 0;

    /**
     * Add a new import function for the given file extension.
     */
    virtual void AddImportFunction(const std::string& extension, AssetImportFunction importFunction) = 0;
  };

}
