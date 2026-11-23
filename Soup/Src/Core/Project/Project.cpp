#include "Project.h"

#include "Asset/AssetManagerSystem.h"
#include "ProjectSerializer.h"

namespace Soup
{

  Ref<Project> Project::Load(const std::filesystem::path& projectPath)
  {
    Ref<Project> project = CreateRef<Project>();
    ProjectSerializer serializer(project);

    if (serializer.Deserialize(projectPath))
    {
      s_ActiveProject = project;

      s_ActiveProject->m_AssetManagerSystem = CreateRef<AssetManagerSystem>();
      std::filesystem::path assetRegistryPath = GetActiveWorkingDirectory() / "AssetRegistry.spareg";
      s_ActiveProject->m_AssetManagerSystem->LoadAssetRegistry(assetRegistryPath);

      return s_ActiveProject;
    }

    return nullptr;
  }

  void Project::Save(const std::filesystem::path& projectPath)
  {
    std::filesystem::path assetRegistryPath = GetActiveWorkingDirectory() / "AssetRegistry.spareg";
    s_ActiveProject->m_AssetManagerSystem->SaveAssetRegistry(assetRegistryPath);

    ProjectSerializer serializer(s_ActiveProject);
    serializer.Serialize(projectPath);
  }

  void Project::SaveActiveProject()
  {
    std::filesystem::path projectFilePath = GetActiveWorkingDirectory() / (GetActive()->GetConfig().Name + ".spproj");
    Save(projectFilePath);
  }

  Ref<Project> Project::New(const std::filesystem::path& projectPath)
  {
    SP_ASSERT(false, "Cannot create project for now!");
    Ref<Project> project = CreateRef<Project>();
    s_ActiveProject = project;
    return project;
  }

  Project::Project(const ProjectConfig& config)
    : m_Config(config)
  {
  }

}
