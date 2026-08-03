#pragma once

namespace Soup
{
  struct UniformBufferSpecifications
  {
    std::string BlockName = "<Unknown>";
    uint32_t BindingPoint = 0;

    UniformBufferSpecifications() = default;
  };

  class UniformBuffer
  {
  public:
    static Ref<UniformBuffer> Create(const UniformBufferSpecifications& specs, uint32_t size);
    static Ref<UniformBuffer> Create(const UniformBufferSpecifications& specs, uint32_t size, const void* data);

    virtual ~UniformBuffer() = default;

    virtual void SetData(const void* data) = 0;
    virtual void Resize(uint32_t newSize) = 0;

    virtual const UniformBufferSpecifications& GetSpecifications() const = 0;
  };

}
