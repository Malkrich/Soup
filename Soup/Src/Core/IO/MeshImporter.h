#pragma once

#include "Geometry/Mesh.h"
#include "MeshReader.h"

namespace Soup
{

  class MeshImporter
  {
  public:
    static void Init();
    static void Shutdown();

    /**
     * Add a reader for the corresponding extension.
     */
    template <typename T>
    static void AddReader(const std::string& extension) requires(std::is_base_of<MeshReader, T>::value)
    {
      Ref<T> reader = CreateRef<T>();
      AddReaderImpl(extension, reader);
    }

    /**
     * Load a mesh at the given path.
     * If the import fails, it returns a nullptr.
     */
    static Ref<Mesh> Import(const std::filesystem::path& filePath);

  private:
    static void AddReaderImpl(const std::string& extension, Ref<MeshReader> reader);
  };

}
