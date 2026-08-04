#pragma once

#include "Renderer/ShaderStorageBuffer.h"

namespace Soup
{

  class OpenGLShaderStorageBuffer : public ShaderStorageBuffer
  {
  public:
    OpenGLShaderStorageBuffer(const ShaderStorageBufferSpecifications& specs, uint32_t size, const void* data);
    OpenGLShaderStorageBuffer(const ShaderStorageBufferSpecifications& specs, uint32_t size);
    virtual ~OpenGLShaderStorageBuffer();

    virtual void Bind() const override;

    virtual uint32_t GetSize() const override { return m_BufferSize; }

    virtual void Resize(uint32_t size) override;
    virtual void SetData(const void* data) override;

  private:
    void Invalidate(uint32_t size, const void* data);

  private:
    uint32_t m_BufferID = 0;
    uint32_t m_BufferSize = 0;

    ShaderStorageBufferSpecifications m_Specs;
  };

}
