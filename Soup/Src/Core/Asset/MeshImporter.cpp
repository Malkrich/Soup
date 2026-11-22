#include "MeshImporter.h"

#include "IO/MeshReader.h"

namespace Soup
{

  MeshImporter::MeshImporter()
  {
    m_ImportFunctions[".obj"] = MeshReader::ImportObjMesh;
  }

  Ref<Asset> MeshImporter::Import(const std::filesystem::path& filePath)
  {
    std::string extension = filePath.extension().string();
    auto it = m_ImportFunctions.find(extension);
    if (it == m_ImportFunctions.end())
    {
      SP_LOG_ERROR("Extension {} is not supported.", filePath.extension().generic_string());
      return nullptr;
    }

    return it->second(filePath);
  }

  bool MeshImporter::CanImport(const std::filesystem::path& filePath) const
  {
    auto it = m_ImportFunctions.find(filePath.extension().string());
    return it != m_ImportFunctions.end();
  }

  void MeshImporter::AddImportFunction(const std::string& extension, Importer::AssetImportFunction function)
  {
    m_ImportFunctions[extension] = function;
  }
}
