#include "OpenGLShaderStorageBuffer.h"

#include "OpenGLError.h"

#include <glad/glad.h>

namespace Soup
{

  OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(
    const ShaderStorageBufferSpecifications& specs, uint32_t size, const void* data)
    : m_Specs(specs)
  {
    Invalidate(size, data);
  }

  OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(const ShaderStorageBufferSpecifications& specs, uint32_t size)
    : m_Specs(specs)
  {
    Invalidate(size, nullptr);
  }

  OpenGLShaderStorageBuffer::~OpenGLShaderStorageBuffer()
  {
    glDeleteBuffers(1, &m_BufferID);
  }

  void OpenGLShaderStorageBuffer::Bind() const
  {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferID);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_Specs.BindingPoint, m_BufferID);
  }

  void OpenGLShaderStorageBuffer::Resize(uint32_t size)
  {
    Invalidate(size, nullptr);
  }

  void OpenGLShaderStorageBuffer::SetData(const void* data)
  {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_BufferSize, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    CHECK_GL_ERROR();
  }

  void OpenGLShaderStorageBuffer::Invalidate(uint32_t size, const void* data)
  {
    m_BufferSize = size;
    if (m_BufferID)
    {
      glDeleteBuffers(1, &m_BufferID);
    }

    glGenBuffers(1, &m_BufferID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_BufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_BufferSize, data, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_Specs.BindingPoint, m_BufferID);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    CHECK_GL_ERROR();
  }

}
