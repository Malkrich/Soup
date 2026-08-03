#include "VertexBuffer.h"

#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLVertexBuffer.h"

namespace Soup
{

  namespace Utils
  {
    using BufferElementType = Soup::BufferElementType;

    static uint32_t GetElementSize(BufferElementType type)
    {
      switch (type)
      {
        case BufferElementType::Bool:
          return 1;
        case BufferElementType::Int:
          return 1 * sizeof(int);
        case BufferElementType::Int2:
          return 2 * sizeof(int);
        case BufferElementType::Int3:
          return 3 * sizeof(int);
        case BufferElementType::Int4:
          return 4 * sizeof(int);
        case BufferElementType::Float:
          return 1 * sizeof(float);
        case BufferElementType::Float2:
          return 2 * sizeof(float);
        case BufferElementType::Float3:
          return 3 * sizeof(float);
        case BufferElementType::Float4:
          return 4 * sizeof(float);
        case BufferElementType::Mat2:
          return 2 * 2 * sizeof(float);
        case BufferElementType::Mat3:
          return 3 * 3 * sizeof(float);
        case BufferElementType::Mat4:
          return 4 * 4 * sizeof(float);
        default:
          SP_ASSERT(false, "Buffer element type unknown !");
          return 0;
      }
    }
  }

  Layout::Layout(const std::initializer_list<BufferElement>& elements)
    : m_Elements(elements)
  {
    ComputeOffsetAndStride();
  }

  void Layout::ComputeOffsetAndStride()
  {
    uint32_t offset = 0;
    for (auto& element : m_Elements)
    {
      element.Offset = offset;
      uint32_t elementSize = Utils::GetElementSize(element.Type);
      m_Stride += elementSize;
      offset += elementSize;
    }
  }

  Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, const void* data)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLVertexBuffer>(size, data);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

  Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLVertexBuffer>(size);
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

}
