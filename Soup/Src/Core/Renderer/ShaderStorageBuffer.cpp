#include "Renderer.h"

#include "ShaderStorageBuffer.h"

#include "Platforms/OpenGL/OpenGLShaderStorageBuffer.h"

namespace Soup
{

  Ref<ShaderStorageBuffer> ShaderStorageBuffer::Create(
    const ShaderStorageBufferSpecifications& specs, uint32_t size, const void* data)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLShaderStorageBuffer>(specs, size, data);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

  Ref<ShaderStorageBuffer> ShaderStorageBuffer::Create(const ShaderStorageBufferSpecifications& specs, uint32_t size)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLShaderStorageBuffer>(specs, size);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

}
