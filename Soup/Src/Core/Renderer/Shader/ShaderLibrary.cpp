#include "ShaderLibrary.h"

namespace Soup
{

  const Ref<Shader>& ShaderLibrary::Get(const std::string& name) const
  {
    SP_ASSERT(m_ShaderRegistry.find(name) != m_ShaderRegistry.end(), "Shader not found!");
    const ShaderData& sData = m_ShaderRegistry.at(name);
    return sData.ShaderProgram;
  }

  void ShaderLibrary::Add(const ShaderData& shaderData)
  {
    const std::string& name = shaderData.ShaderProgram->GetName();
    SP_ASSERT(m_ShaderRegistry.find(name) == m_ShaderRegistry.end(), "Shader already registered!");

    m_ShaderRegistry[name] = shaderData;
  }

  void ShaderLibrary::Load(const std::filesystem::path& filePath)
  {
    ShaderData shaderData;
    shaderData.ShaderProgram = Shader::Create(filePath);
    shaderData.FilePath = filePath;
    Add(shaderData);
  }

  void ShaderLibrary::Reload(const std::string& shaderName)
  {
    SP_ASSERT(m_ShaderRegistry.find(shaderName) != m_ShaderRegistry.end(), "Shader not found !");

    ShaderData& sData = m_ShaderRegistry.at(shaderName);
    sData.ShaderProgram = Shader::Create(sData.FilePath);
  }

}
