#include "MeshImporter.h"

#include "ObjMeshReader.h"

namespace Soup
{

  struct MeshImportData
  {
    std::unordered_map<std::string, Ref<MeshReader>> LoaderMap = { { ".obj", CreateRef<ObjMeshReader>() } };
  };

  static MeshImportData* s_MeshImporterData = nullptr;

  void MeshImporter::Init()
  {
    s_MeshImporterData = new MeshImportData();
  }

  void MeshImporter::Shutdown()
  {
    delete s_MeshImporterData;
    s_MeshImporterData = nullptr;
  }

  void MeshImporter::AddReaderImpl(const std::string& extension, Ref<MeshReader> reader)
  {
    s_MeshImporterData->LoaderMap[extension] = reader;
  }

  Ref<Mesh> MeshImporter::Import(const std::filesystem::path& filePath)
  {
    std::string extension = filePath.extension().string();
    auto it = s_MeshImporterData->LoaderMap.find(extension);
    if (it == s_MeshImporterData->LoaderMap.end())
    {
      SP_LOG_ERROR("Extension {} is not supported.", filePath.extension().generic_string());
      return nullptr;
    }

    bool success = it->second->Read(filePath);
    if (!success)
    {
      SP_LOG_ERROR("Import of {} failed.", filePath.generic_string());
      return nullptr;
    }

    return it->second->GetMesh();
  }
}
