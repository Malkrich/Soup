#pragma once

#include "FrameBuffer.h"
#include "Pipeline.h"
#include "ShaderStorageBuffer.h"
#include "Texture2D.h"
#include "TextureCubemap.h"
#include "UniformBuffer.h"

#include <glm/glm.hpp>

namespace Soup
{

  struct RenderPassSpecifications
  {
    // Pipeline RenderPassPipeline;
    std::string Name;

    Ref<FrameBuffer> FrameBuffer;
    Ref<Pipeline> Pipeline;
  };

  class RenderPass
  {
  public:
    static Ref<RenderPass> Create(const RenderPassSpecifications& specs);

    virtual void Begin() = 0;
    virtual void End() = 0;

    virtual Ref<FrameBuffer> GetFrameBuffer() = 0;
    virtual Ref<Pipeline> GetPipeline() = 0;

    virtual void RemoveUniformInput(const std::string& name) = 0;

    virtual void SetInput(uint32_t index, Ref<Texture> texture) = 0;
    virtual void SetInput(const std::string& name, Ref<UniformBuffer> texture) = 0;
    virtual void SetInput(const std::string& name, Ref<ShaderStorageBuffer> texture) = 0;

    virtual Ref<Texture2D> GetOutput(uint32_t index) const = 0;
    virtual Ref<Texture2D> GetDepthOutput() const = 0;
  };

}
