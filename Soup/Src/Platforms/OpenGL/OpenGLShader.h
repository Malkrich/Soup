#pragma once

#include "Renderer/Shader/Shader.h"

namespace Soup
{

  class OpenGLShader : public Shader
  {
  public:
    OpenGLShader(const std::filesystem::path& filePath);

    virtual ~OpenGLShader();

    virtual const std::string& GetName() const override { return m_Name; }

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void Reload(const std::filesystem::path& filePath) override { Invalidate(filePath); }

    virtual void SetBool(const std::string& name, bool data) override;
    virtual void SetFloat3(const std::string& name, const glm::vec3& data) override;
    virtual void SetMat4(const std::string& name, const glm::mat4& data) override;

  private:
    void Invalidate(const std::filesystem::path& filePath);

    uint32_t CompileShader(const std::string& shaderContent, ShaderType shaderType);
    uint32_t CompileAndLink(const std::unordered_map<ShaderType, std::string>& shaderSources);

  private:
    std::string m_Name;
    uint32_t m_ShaderProgramID = 0;
  };
}
