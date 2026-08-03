#pragma once

namespace Soup
{

  class IndexBuffer
  {
  public:
    static Ref<IndexBuffer> Create(uint32_t size, const void* data);
    static Ref<IndexBuffer> Create(uint32_t size);

    virtual ~IndexBuffer() = default;

    virtual uint32_t GetCount() const = 0;

    virtual void SetData(uint32_t size, void* data) = 0;
  };

}
