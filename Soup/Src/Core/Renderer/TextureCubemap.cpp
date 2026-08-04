#include "Renderer.h"

#include "TextureCubemap.h"

#include "Platforms/OpenGL/OpenGLTextureCubemap.h"

namespace Soup
{

  Ref<TextureCubemap> TextureCubemap::Create(const TextureCubemapSpecifications& specs)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLTextureCubemap>(specs);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

  Ref<TextureCubemap> TextureCubemap::Copy(const TextureCubemapSpecifications& specs, uint32_t textureID)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return OpenGLTextureCubemap::Copy(specs, textureID);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

}
