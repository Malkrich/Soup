#pragma once

#include "MeshReader.h"

namespace Soup
{

  class ObjMeshReader : public MeshReader
  {
  public:
    bool Read(const std::filesystem::path& filePath) override;
  };

}
