#pragma once

#include "Renderer/RenderPass.h"

#include "OpenGLShaderStorageBuffer.h"
#include "OpenGLTexture2D.h"
#include "OpenGLTextureCubemap.h"
#include "OpenGLUniformBuffer.h"

namespace Soup
{

  class OpenGLRenderPass : public RenderPass
  {
  public:
    OpenGLRenderPass(const RenderPassSpecifications& specs);

    virtual void Begin() override;
    virtual void End() override;

    virtual Ref<FrameBuffer> GetFrameBuffer() override { return m_Specs.FrameBuffer; }
    virtual Ref<Pipeline> GetPipeline() override { return m_Specs.Pipeline; }

    virtual void RemoveUniformInput(const std::string& name) override;

    virtual void SetInput(uint32_t index, Ref<Texture> texture) override;
    virtual void SetInput(const std::string& name, Ref<UniformBuffer> texture) override;
    virtual void SetInput(const std::string& name, Ref<ShaderStorageBuffer> texture) override;

    virtual Ref<Texture2D> GetOutput(uint32_t index) const override;
    virtual Ref<Texture2D> GetDepthOutput() const override;

  private:
    RenderPassSpecifications m_Specs;

    std::unordered_map<uint32_t, Ref<Texture>> m_TextureInputs;
    std::unordered_map<std::string, Ref<OpenGLUniformBuffer>> m_UniformBufferInputs;
    std::unordered_map<std::string, Ref<OpenGLShaderStorageBuffer>> m_ShaderStorageBufferInputs;
  };

}
