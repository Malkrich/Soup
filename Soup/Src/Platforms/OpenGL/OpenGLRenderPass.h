#pragma once

#include "Renderer/RenderPass.h"

#include "OpenGLShaderStorageBuffer.h"
#include "OpenGLUniformBuffer.h"

namespace Soup
{

  class OpenGLRenderPass : public RenderPass
  {
  public:
    OpenGLRenderPass(const RenderPassSpecifications& specs);

    void Begin() override;
    void End() override;

    const RenderPassSpecifications& GetSpecifications() const override { return m_Specs; }
    Ref<FrameBuffer> GetFrameBuffer() override { return m_Specs.FrameBufferObject; }
    Ref<Pipeline> GetPipeline() override { return m_Specs.PipelineObject; }

    void RemoveUniformInput(const std::string& name) override;

    void SetInput(uint32_t index, Ref<Texture> texture) override;
    void SetInput(const std::string& name, Ref<UniformBuffer> texture) override;
    void SetInput(const std::string& name, Ref<ShaderStorageBuffer> texture) override;

    Ref<Texture2D> GetOutput(uint32_t index) const override;
    Ref<Texture2D> GetDepthOutput() const override;

  private:
    RenderPassSpecifications m_Specs;

    std::unordered_map<uint32_t, Ref<Texture>> m_TextureInputs;
    std::unordered_map<std::string, Ref<OpenGLUniformBuffer>> m_UniformBufferInputs;
    std::unordered_map<std::string, Ref<OpenGLShaderStorageBuffer>> m_ShaderStorageBufferInputs;
  };

}
