#pragma once

#include "Geometry/Mesh.h"

namespace Soup
{

  class MeshReader
  {
  public:
    MeshReader() = default;
    virtual ~MeshReader() = default;

    virtual bool Read(const std::filesystem::path& filePath) = 0;

    Ref<Mesh> GetMesh() const { return m_Mesh; }

  protected:
    Ref<Mesh> m_Mesh = nullptr;
  };

}
