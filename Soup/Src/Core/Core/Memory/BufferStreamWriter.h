#pragma once

#include "Buffer.h"

namespace Soup
{

  class BufferStreamWriter
  {
  public:
    BufferStreamWriter(Buffer& buffer);

    template <typename T>
    void WritePrimitiveType(T data)
    {
      uint32_t typeSize = sizeof(T);
      m_Buffer.Resize(m_Buffer.Size + typeSize);

      memcpy(m_Buffer.Data + m_BufferPointer, &data, typeSize);

      m_BufferPointer += typeSize;
    }

    template <typename T>
    void WriteSpan(uint32_t count, T data)
    {
      for (uint32_t i = 0; i < count; i++)
        WritePrimitiveType<T>(data);
    }

    template <typename T>
    void WriteVector(const std::vector<T>& vector)
    {
      uint32_t vectorSize = sizeof(T) * vector.size();
      m_Buffer.Resize(m_Buffer.Size + vectorSize);

      memcpy(m_Buffer.Data + m_BufferPointer, vector.data(), vectorSize);
      m_BufferPointer += vectorSize;
    }

  private:
    Buffer& m_Buffer;
    uint32_t m_BufferPointer = 0;
  };

}
