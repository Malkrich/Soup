#pragma once

#include "Project.h"

namespace Soup
{

  class ProjectSerializer
  {
  public:
    ProjectSerializer() = default;
    ProjectSerializer(Ref<Project> project);

    bool Serialize(const std::filesystem::path& filePath) const;
    bool Deserialize(const std::filesystem::path& filePath);

  private:
    Ref<Project> m_Project = nullptr;
  };

}
