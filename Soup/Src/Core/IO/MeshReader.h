#pragma once

namespace Soup
{

  class Asset;

  /**
   * Readers supported by Soup for meshes.
   * Import functions return the mesh at the given file path.
   * Return nullptr when the import fails.
   */
  class MeshReader
  {
  public:
    static Ref<Asset> ImportObjMesh(const std::filesystem::path& filePath);
  };

}
