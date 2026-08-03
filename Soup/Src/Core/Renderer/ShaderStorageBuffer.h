#pragma once

namespace Soup
{

  struct ShaderStorageBufferSpecifications
  {
    std::string BlockName = "<Unknown>";
    uint32_t BindingPoint = 0;
  };

  class ShaderStorageBuffer
  {
  public:
    static Ref<ShaderStorageBuffer> Create(
      const ShaderStorageBufferSpecifications& specs, uint32_t size, const void* data);
    static Ref<ShaderStorageBuffer> Create(const ShaderStorageBufferSpecifications& specs, uint32_t size);

    virtual ~ShaderStorageBuffer() = default;

    virtual void Bind() const = 0;

    virtual uint32_t GetSize() const = 0;

    virtual void Resize(uint32_t size) = 0;
    virtual void SetData(const void* data) = 0;
  };

}
