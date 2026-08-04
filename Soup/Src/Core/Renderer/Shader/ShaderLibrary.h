#pragma once

#include "Renderer/Shader/Shader.h"

#include <filesystem>

namespace Soup
{

  struct ShaderData
  {
    std::filesystem::path FilePath;
    Ref<Shader> ShaderProgram;
  };

  class ShaderLibrary
  {
  public:
    const Ref<Shader>& Get(const std::string& name) const;

    void Add(const ShaderData& shader);

    void Load(const std::filesystem::path& filePath);
    void Reload(const std::string& shaderName);

    std::unordered_map<std::string, ShaderData>::const_iterator begin() const { return m_ShaderRegistry.begin(); }
    std::unordered_map<std::string, ShaderData>::const_iterator end() const { return m_ShaderRegistry.end(); }
    std::unordered_map<std::string, ShaderData>::iterator begin() { return m_ShaderRegistry.begin(); }
    std::unordered_map<std::string, ShaderData>::iterator end() { return m_ShaderRegistry.end(); }

  private:
    std::unordered_map<std::string, ShaderData> m_ShaderRegistry;
  };

}
