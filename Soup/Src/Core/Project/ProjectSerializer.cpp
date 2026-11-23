#include "ProjectSerializer.h"

#include <nlohmann/json.hpp>

namespace Soup
{

  namespace Utils
  {
    static bool IsJsonValid(nlohmann::json json)
    {
      auto projecIt = json.find("Project");
      if (projecIt == json.end())
      {
        return false;
      }

      bool isValid = projecIt.value().find("Name") != projecIt.value().end();
      isValid &= projecIt.value().find("WorkingDirectory") != projecIt.value().end();
      return isValid;
    }
  }

  ProjectSerializer::ProjectSerializer(Ref<Project> project)
    : m_Project(project)
  {
  }

  bool ProjectSerializer::Serialize(const std::filesystem::path& filePath) const
  {
    nlohmann::json jsonRoot;
    nlohmann::json& projectNode = jsonRoot["Project"];
    projectNode["Name"] = m_Project->GetConfig().Name;
    projectNode["WorkingDirectory"] = m_Project->GetConfig().WorkingDirectory.generic_string();

    std::ofstream fileStream(filePath);
    fileStream << jsonRoot.dump(4) + '\n';
    return true;
  }

  bool ProjectSerializer::Deserialize(const std::filesystem::path& filePath)
  {
    if (!std::filesystem::exists(filePath))
    {
      SP_LOG_ERROR("Could not find project file: {}.", filePath.generic_string());
      return false;
    }

    std::ifstream file(filePath);
    nlohmann::json jsonRoot = nlohmann::json::parse(file);

    if (!Utils::IsJsonValid(jsonRoot))
    {
      SP_LOG_ERROR("Project file {} does not have a valid format!", filePath.generic_string());
      return false;
    }

    nlohmann::json projectNode = jsonRoot["Project"];
    ProjectConfig& config = m_Project->GetConfig();
    config.Name = projectNode["Name"];
    config.WorkingDirectory = std::string(projectNode["WorkingDirectory"]);

    return true;
  }

}
