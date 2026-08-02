#include "OpenGLUniformBuffer.h"

#include "OpenGLError.h"

#include <glad/glad.h>

namespace Soup
{

  OpenGLUniformBuffer::OpenGLUniformBuffer(const UniformBufferSpecifications& specs, uint32_t size)
    : m_Specs(specs)
  {
    Invalidate(size, nullptr);
  }

  OpenGLUniformBuffer::OpenGLUniformBuffer(const UniformBufferSpecifications& specs, uint32_t size, const void* data)
    : m_Specs(specs)
  {
    Invalidate(size, data);
  }

  OpenGLUniformBuffer::~OpenGLUniformBuffer()
  {
    glDeleteBuffers(1, &m_BufferID);
  }

  void OpenGLUniformBuffer::Bind() const
  {
    glBindBuffer(GL_UNIFORM_BUFFER, m_BufferID);
    glBindBufferBase(GL_UNIFORM_BUFFER, m_Specs.BindingPoint, m_BufferID);
  }

  void OpenGLUniformBuffer::SetData(const void* data)
  {
    glBindBuffer(GL_UNIFORM_BUFFER, m_BufferID);
    glBufferData(GL_UNIFORM_BUFFER, m_BufferSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    CHECK_GL_ERROR();
  }

  void OpenGLUniformBuffer::Resize(uint32_t newSize)
  {
    Invalidate(newSize, nullptr);
  }

  void OpenGLUniformBuffer::Invalidate(uint32_t size, const void* data)
  {
    m_BufferSize = size;
    if (m_BufferID)
    {
      glDeleteBuffers(1, &m_BufferID);
    }

    glGenBuffers(1, &m_BufferID);
    glBindBuffer(GL_UNIFORM_BUFFER, m_BufferID);
    glBufferData(GL_UNIFORM_BUFFER, m_BufferSize, data, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, m_Specs.BindingPoint, m_BufferID);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    CHECK_GL_ERROR();
  }

}
