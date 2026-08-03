#include "Renderer.h"

#include "UniformBuffer.h"

#include "Platforms/OpenGL/OpenGLUniformBuffer.h"

namespace Soup
{

  Ref<UniformBuffer> UniformBuffer::Create(const UniformBufferSpecifications& specs, uint32_t size)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLUniformBuffer>(specs, size);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

  Ref<UniformBuffer> UniformBuffer::Create(const UniformBufferSpecifications& specs, uint32_t size, const void* data)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLUniformBuffer>(specs, size, data);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

}
