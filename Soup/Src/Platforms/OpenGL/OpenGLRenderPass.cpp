#include "OpenGLRenderPass.h"

#include "OpenGLFrameBuffer.h"
#include "OpenGLPipeline.h"
#include "OpenGLTexture2D.h"
#include "OpenGLTextureCubemap.h"

#include <glad/glad.h>

namespace Soup
{

  OpenGLRenderPass::OpenGLRenderPass(const RenderPassSpecifications& specs)
    : m_Specs(specs)
  {
  }

  void OpenGLRenderPass::Begin()
  {
    glViewport(0, 0, m_Specs.FrameBufferObject->GetWidth(), m_Specs.FrameBufferObject->GetHeight());

    SP_ASSERT(std::dynamic_pointer_cast<OpenGLFrameBuffer>(m_Specs.FrameBufferObject),
      "OpenGLRenderPass only supports OpenGLFrameBuffer");
    std::dynamic_pointer_cast<OpenGLFrameBuffer>(m_Specs.FrameBufferObject)->Bind();
    m_Specs.FrameBufferObject->ClearAttachmentsIfNeeded();

    for (auto& [index, texture] : m_TextureInputs)
    {
      if (std::dynamic_pointer_cast<OpenGLTexture2D>(texture))
      {
        std::dynamic_pointer_cast<OpenGLTexture2D>(texture)->Bind(index);
      }
      else
      {
        std::dynamic_pointer_cast<OpenGLTextureCubemap>(texture)->Bind(index);
      }
    }

    for (auto& [_, uniformBuffer] : m_UniformBufferInputs)
    {
      uniformBuffer->Bind();
    }

    for (auto& [_, shaderStorageBuffer] : m_ShaderStorageBufferInputs)
    {
      shaderStorageBuffer->Bind();
    }

    uint32_t width = m_Specs.FrameBufferObject->GetWidth();
    uint32_t height = m_Specs.FrameBufferObject->GetHeight();
    std::dynamic_pointer_cast<OpenGLPipeline>(m_Specs.PipelineObject)->Bind(width, height);
  }

  void OpenGLRenderPass::End()
  {
    SP_ASSERT(std::dynamic_pointer_cast<OpenGLFrameBuffer>(m_Specs.FrameBufferObject),
      "OpenGLRenderPass supports only OpenGLFrameBuffer!");
    std::dynamic_pointer_cast<OpenGLFrameBuffer>(m_Specs.FrameBufferObject)->Unbind();
  }

  void OpenGLRenderPass::RemoveUniformInput(const std::string& name)
  {
    m_UniformBufferInputs.erase(name);
  }

  void OpenGLRenderPass::SetInput(uint32_t index, Ref<Texture> texture)
  {
    SP_ASSERT(
      std::dynamic_pointer_cast<OpenGLTexture2D>(texture) || std::dynamic_pointer_cast<OpenGLTextureCubemap>(texture),
      "OpenGLRenderPass only supports OpenGLTexture2D or OpenGLCubemap!");
    m_TextureInputs[index] = texture;
  }

  void OpenGLRenderPass::SetInput(const std::string& name, Ref<UniformBuffer> texture)
  {
    SP_ASSERT(
      std::dynamic_pointer_cast<OpenGLUniformBuffer>(texture), "OpenGLRenderPass only supports OpenGLUniformBuffer!");
    m_UniformBufferInputs[name] = std::dynamic_pointer_cast<OpenGLUniformBuffer>(texture);
  }

  void OpenGLRenderPass::SetInput(const std::string& name, Ref<ShaderStorageBuffer> texture)
  {
    SP_ASSERT(std::dynamic_pointer_cast<OpenGLShaderStorageBuffer>(texture),
      "OpenGLRenderPass only supports OpenGLShaderStorageBuffer!");
    m_ShaderStorageBufferInputs[name] = std::dynamic_pointer_cast<OpenGLShaderStorageBuffer>(texture);
  }

  Ref<Texture2D> OpenGLRenderPass::GetOutput(uint32_t index) const
  {
    Ref<Texture2D> texture = std::dynamic_pointer_cast<Texture2D>(
      m_Specs.FrameBufferObject->GetColorAttachmentSpecs(index).ExistingAttachment);
    SP_ASSERT(texture, "Output has to be texture 2D!");
    return texture;
  }

  Ref<Texture2D> OpenGLRenderPass::GetDepthOutput() const
  {
    Ref<Texture2D> texture =
      std::dynamic_pointer_cast<Texture2D>(m_Specs.FrameBufferObject->GetDepthAttachmentSpecs().ExistingAttachment);
    SP_ASSERT(texture, "Output has to be a texture 2D");
    return texture;
  }

}
