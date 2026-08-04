#pragma once

#include "Renderer/UniformBuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Soup
{

  enum class ShaderType
  {
    None = 0,
    VertexShader,
    FragmentShader
  };

  class Shader
  {
  public:
    static Ref<Shader> Create(const std::filesystem::path& filePath);

    virtual ~Shader() = default;

    virtual const std::string& GetName() const = 0;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void Reload(const std::filesystem::path& filePath) = 0;

    virtual void SetBool(const std::string& name, bool data) = 0;
    virtual void SetFloat3(const std::string& name, const glm::vec3& data) = 0;
    virtual void SetMat4(const std::string& name, const glm::mat4& data) = 0;
  };

}
