#include "Buffer.h"

namespace Soup
{

  Buffer::Buffer(uint32_t size)
  {
    Allocate(size);
  }

  void Buffer::Release()
  {
    delete[] Data;
    Data = nullptr;
    Size = 0;
  }

  void Buffer::Allocate(uint32_t size)
  {
    Release();

    Data = new uint8_t[size];
    Size = size;
  }

  void Buffer::Resize(uint32_t size)
  {
    // Create new buffer and copy actual data in
    Buffer newBuffer(size);
    memcpy(newBuffer.Data, Data, Size);

    // Free current buffer memory
    Release();
    // Assign new buffer data ans size to the current buffer
    *this = newBuffer;
  }

  Buffer Buffer::Copy(const Buffer& other)
  {
    SP_ASSERT(other, "Buffer source does not contain data to copy !");

    Buffer newBuffer;
    newBuffer.Size = other.Size;
    memcpy(newBuffer.Data, other.Data, (size_t)newBuffer.Size);

    return newBuffer;
  }

}
