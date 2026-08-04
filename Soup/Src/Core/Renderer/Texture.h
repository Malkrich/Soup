#pragma once

#include <cstdint>

namespace Soup
{

  class Texture
  {
  public:
    virtual ~Texture() = default;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual uint32_t GetHandle() const = 0;
  };

}
