#pragma once

#include "Renderer/VertexBuffer.h"

namespace Soup
{

  class OpenGLVertexBuffer : public VertexBuffer
  {
  public:
    OpenGLVertexBuffer(uint32_t size, const void* data);
    OpenGLVertexBuffer(uint32_t size);

    virtual ~OpenGLVertexBuffer();

    void Bind() const;
    void Unbind() const;

    virtual void SetData(uint32_t size, const void* data) override;

    virtual const Layout& GetGeometryLayout() const override { return m_GeometryLayout; }
    virtual void SetGeometryLayout(const Layout& layout) override { m_GeometryLayout = layout; }
    virtual const Layout& GetInstanceLayout() const override { return m_InstanceLayout; }
    virtual void SetInstanceLayout(const Layout& layout) override { m_InstanceLayout = layout; }

  private:
    uint32_t m_BufferID;

    Layout m_GeometryLayout;
    Layout m_InstanceLayout;
  };

}
