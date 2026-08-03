#include "Renderer/Renderer.h"

#include "Shader.h"

#include "Platforms/OpenGL/OpenGLShader.h"

namespace Soup
{

  Ref<Shader> Shader::Create(const std::filesystem::path& filePath)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLShader>(filePath);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

}
