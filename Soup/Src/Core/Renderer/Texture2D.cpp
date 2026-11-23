#include "Renderer.h"

#include "Texture2D.h"

#include "Platforms/OpenGL/OpenGLTexture2D.h"

namespace Soup
{

  Ref<Texture2D> Texture2D::Create(const Texture2DSpecifications specs, void* data)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLTexture2D>(specs, data);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

  Ref<Texture2D> Texture2D::Copy(const Texture2DSpecifications& specs, uint32_t sourceTexture)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return OpenGLTexture2D::Copy(specs, sourceTexture);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

}
