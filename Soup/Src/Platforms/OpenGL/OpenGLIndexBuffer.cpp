#include "OpenGLIndexBuffer.h"

#include "OpenGLError.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Soup
{

  OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t size, const void* data)
    : m_Count(size / sizeof(uint32_t))
  {
    glGenBuffers(1, &m_BufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    CHECK_GL_ERROR();
  }

  OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t size)
    : m_Count(size / sizeof(uint32_t))
  {
    glGenBuffers(1, &m_BufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

    CHECK_GL_ERROR();
  }

  OpenGLIndexBuffer::~OpenGLIndexBuffer()
  {
    glDeleteBuffers(1, &m_BufferID);
  }

  void OpenGLIndexBuffer::SetData(uint32_t size, void* data)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
    // We first need to invalidate the buffer data and the copy the new buffer data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    m_Count = size / sizeof(unsigned int);
  }

  void OpenGLIndexBuffer::Bind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
  }

  void OpenGLIndexBuffer::Unbind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

}
