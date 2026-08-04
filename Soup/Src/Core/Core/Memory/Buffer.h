#pragma once

namespace Soup
{

  struct Buffer
  {
    uint32_t Size = 0;
    uint8_t* Data = nullptr;

    Buffer() = default;
    Buffer(const Buffer&) = default;
    Buffer(uint32_t size);

    static Buffer Copy(const Buffer& other);

    void Release();
    void Allocate(uint32_t size);
    void Resize(uint32_t size);

    operator bool() const { return Data != nullptr; }
  };

}
