#include "OpenGLVertexArray.h"

#include "OpenGLError.h"
#include "OpenGLIndexBuffer.h"
#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Soup
{

  namespace Utils
  {
    using BufferElementType = Soup::BufferElementType;

    static uint32_t GetElementCount(BufferElementType type)
    {
      switch (type)
      {
        case BufferElementType::Bool:
          return 1;
        case BufferElementType::Int:
          return 1;
        case BufferElementType::Int2:
          return 2;
        case BufferElementType::Int3:
          return 3;
        case BufferElementType::Int4:
          return 4;
        case BufferElementType::Float:
          return 1;
        case BufferElementType::Float2:
          return 2;
        case BufferElementType::Float3:
          return 3;
        case BufferElementType::Float4:
          return 4;
        case BufferElementType::Mat2:
          return 2 * 2;
        case BufferElementType::Mat3:
          return 3 * 3;
        case BufferElementType::Mat4:
          return 4 * 4;
      }

      SP_ASSERT(false, "Buffer element type unknown !");
      return 0;
    }

    static void EnableFloatAttribute(uint32_t index, const BufferElement& element, uint32_t stride, bool instanceLayout)
    {
      uint32_t elementCount = Utils::GetElementCount(element.Type);
      glVertexAttribPointer(index, elementCount, GL_FLOAT, element.Normalized ? GL_TRUE : GL_FALSE, stride,
        reinterpret_cast<const void*>((uint64_t)element.Offset));
      glEnableVertexAttribArray(index);

      if (instanceLayout)
      {
        glVertexAttribDivisor(index, 1);
      }
    }

    static void EnableIntegerAttribute(
      uint32_t index, const BufferElement& element, uint32_t stride, bool instanceLayout)
    {
      uint32_t elementCount = Utils::GetElementCount(element.Type);
      glVertexAttribIPointer(
        index, elementCount, GL_INT, stride, reinterpret_cast<const void*>((uint64_t)element.Offset));
      glEnableVertexAttribArray(index);

      if (instanceLayout)
      {
        glVertexAttribDivisor(index, 1);
      }
    }

    static void EnableMat4Attribute(uint32_t index, const BufferElement& element, uint32_t stride, bool instanceLayout)
    {
      for (uint32_t i = 0; i < 4; i++)
      {
        glVertexAttribPointer(index + i, 4, GL_FLOAT, element.Normalized ? GL_TRUE : GL_FALSE, stride,
          (void*)(element.Offset + i * sizeof(glm::vec4)));
        glEnableVertexAttribArray(index + i);

        if (instanceLayout)
        {
          glVertexAttribDivisor(index + i, 1);
        }
      }
    }
  }

  OpenGLVertexArray::OpenGLVertexArray()
  {
    glCreateVertexArrays(1, &m_BufferID);
  }

  OpenGLVertexArray::~OpenGLVertexArray()
  {
    glDeleteVertexArrays(1, &m_BufferID);
  }

  void OpenGLVertexArray::Bind() const
  {
    glBindVertexArray(m_BufferID);
  }

  void OpenGLVertexArray::Unbind() const
  {
    glBindVertexArray(0);
  }

  void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
  {
    m_VertexBuffers.push_back(vertexBuffer);
  }

  void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
  {
    m_IndexBuffer = indexBuffer;
  }

  void OpenGLVertexArray::Bake()
  {
    uint32_t index = 0;
    for (const Ref<VertexBuffer>& vertexBuffer : m_VertexBuffers)
    {
      Bind();
      std::dynamic_pointer_cast<OpenGLVertexBuffer>(vertexBuffer)->Bind();

      // Geometry layout
      ApplyLayout(index, vertexBuffer->GetGeometryLayout(), false);

      // Instance layout
      ApplyLayout(index, vertexBuffer->GetInstanceLayout(), true);
    }

    if (m_IndexBuffer)
    {
      Bind();
      std::dynamic_pointer_cast<OpenGLIndexBuffer>(m_IndexBuffer)->Bind();
    }
  }

  void OpenGLVertexArray::ApplyLayout(uint32_t& index, const Layout& layout, bool instanceLayout)
  {
    if (layout.IsEmpty())
    {
      return;
    }

    uint32_t stride = layout.GetStride();
    for (const BufferElement& element : layout)
    {
      switch (element.Type)
      {
        case BufferElementType::Mat4:
          Utils::EnableMat4Attribute(index, element, stride, instanceLayout);
          break;
        case BufferElementType::Int:
        case BufferElementType::Int2:
        case BufferElementType::Int3:
        case BufferElementType::Int4:
          Utils::EnableIntegerAttribute(index, element, stride, instanceLayout);
          break;
        case BufferElementType::Float:
        case BufferElementType::Float2:
        case BufferElementType::Float3:
        case BufferElementType::Float4:
          Utils::EnableFloatAttribute(index, element, stride, instanceLayout);
          break;
        default:
          SP_ASSERT(false, "Buffer layout element not implemented yet!");
      }
      index++;
    }
  }

}
