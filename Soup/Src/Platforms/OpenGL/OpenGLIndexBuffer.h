#pragma once

#include "Renderer/IndexBuffer.h"

namespace Soup
{

  class OpenGLIndexBuffer : public IndexBuffer
  {
  public:
    OpenGLIndexBuffer(uint32_t size, const void* data);
    OpenGLIndexBuffer(uint32_t size);

    virtual ~OpenGLIndexBuffer();

    void Bind() const;
    void Unbind() const;
    virtual uint32_t GetCount() const override { return m_Count; }

    virtual void SetData(uint32_t size, void* data) override;

  private:
    uint32_t m_BufferID;
    uint32_t m_Count;
  };

}
