#pragma once

namespace Soup
{

  class AssetManagerSystem;

  struct ProjectConfig
  {
    ProjectConfig() = default;

    std::string Name = "Untitled project";
    std::filesystem::path WorkingDirectory;
  };

  class Project
  {
  public:
    static Ref<Project> GetActive() { return s_ActiveProject; }
    static Ref<Project> Load(const std::filesystem::path& filePath);
    static void Save(const std::filesystem::path& filePath);
    static void SaveActiveProject();
    static Ref<Project> New(const std::filesystem::path& filePath);

    static const std::filesystem::path& GetActiveWorkingDirectory()
    {
      return s_ActiveProject->GetConfig().WorkingDirectory;
    }

    static Ref<AssetManagerSystem> GetActiveAssetManagerSystem() { return s_ActiveProject->m_AssetManagerSystem; }

    Project() = default;
    Project(const ProjectConfig& config);

    ProjectConfig& GetConfig() { return m_Config; }

  private:
    ProjectConfig m_Config;
    Ref<AssetManagerSystem> m_AssetManagerSystem = nullptr;

    inline static Ref<Project> s_ActiveProject = nullptr;
  };

}
