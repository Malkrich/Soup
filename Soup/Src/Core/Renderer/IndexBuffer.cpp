#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLIndexBuffer.h"

namespace Soup
{

  Ref<IndexBuffer> IndexBuffer::Create(uint32_t size, const void* data)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLIndexBuffer>(size, data);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

  Ref<IndexBuffer> IndexBuffer::Create(uint32_t size)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLIndexBuffer>(size);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

}
