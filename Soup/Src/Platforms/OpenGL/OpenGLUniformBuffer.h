#pragma once

#include "Renderer/UniformBuffer.h"

namespace Soup
{

  class OpenGLUniformBuffer : public UniformBuffer
  {
  public:
    OpenGLUniformBuffer(const UniformBufferSpecifications& specs, uint32_t size);
    OpenGLUniformBuffer(const UniformBufferSpecifications& specs, uint32_t size, const void* data);

    virtual ~OpenGLUniformBuffer();

    void Bind() const;

    virtual void SetData(const void* data) override;
    virtual void Resize(uint32_t newSize) override;

    virtual const UniformBufferSpecifications& GetSpecifications() const override { return m_Specs; }

  private:
    void Invalidate(uint32_t size, const void* data);

  private:
    uint32_t m_BufferID = 0;
    uint32_t m_BufferSize = 0;

    UniformBufferSpecifications m_Specs;
  };

}
