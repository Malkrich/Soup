#pragma once

#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexBuffer.h"

namespace Soup
{

  class OpenGLVertexArray
  {
  public:
    OpenGLVertexArray();

    ~OpenGLVertexArray();

    void Bind() const;
    void Unbind() const;

    void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
    void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

    const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() { return m_VertexBuffers; }
    const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

    // Binds all the vertex buffer and index buffer together and create the layout.
    void Bake();

  private:
    void ApplyLayout(uint32_t& index, const Layout& layout, bool instanceLayout);

  private:
    uint32_t m_BufferID;

    std::vector<Ref<VertexBuffer>> m_VertexBuffers;
    Ref<IndexBuffer> m_IndexBuffer;
  };

}
