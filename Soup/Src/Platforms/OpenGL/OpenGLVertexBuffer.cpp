#include "OpenGLVertexBuffer.h"

#include "OpenGLError.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Soup
{

  OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, const void* data)
  {
    glGenBuffers(1, &m_BufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    CHECK_GL_ERROR();
  }

  OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
  {
    glGenBuffers(1, &m_BufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

    CHECK_GL_ERROR();
  }

  OpenGLVertexBuffer::~OpenGLVertexBuffer()
  {
    glDeleteBuffers(1, &m_BufferID);
  }

  void OpenGLVertexBuffer::SetData(uint32_t size, const void* data)
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
    // We first need to invalidate the buffer data and the copy the new buffer data
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
  }

  void OpenGLVertexBuffer::Bind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
  }

  void OpenGLVertexBuffer::Unbind() const
  {
    glBindBuffer(GL_VERTEX_ARRAY, 0);
  }

}
