#pragma once

#include "Renderer/FrameBuffer.h"

namespace Soup
{

  class OpenGLFrameBuffer : public FrameBuffer
  {
  public:
    OpenGLFrameBuffer(const FrameBufferSpecifications& specs);

    virtual ~OpenGLFrameBuffer();

    void Bind() const;
    void Unbind() const;

    void Resize(uint32_t width, uint32_t height) override;
    uint32_t GetWidth() const override { return m_Width; }
    uint32_t GetHeight() const override { return m_Height; }

    const FrameBufferAttachmentSpecs& GetColorAttachmentSpecs(uint32_t attachmentIndex) const override
    {
      return m_ColorAttachmentsSpecs[attachmentIndex];
    }
    const FrameBufferAttachmentSpecs& GetDepthAttachmentSpecs() const override { return m_DepthAttachmentSpecs; }

    void SetClearColorAttachment(uint32_t index, bool clear) override;
    void ClearAttachmentsIfNeeded() override;

    uint32_t GetColorAttachment(uint32_t index) const override;
    void SetColorAttachment(uint32_t index, Ref<Texture> texture) override;

    void AttachCubemapFace(uint32_t attachmentIndex, CubemapFace face, uint32_t mipmapLevel = 0) override;

  private:
    void Invalidate();

    void PopulateAttachments(const std::vector<FrameBufferAttachmentSpecs>& attachments);
    void UpdateSpecsFromTexture(FrameBufferAttachmentSpecs& attachmentSpecs);

  private:
    uint32_t m_BufferID = 0;

    uint32_t m_Width, m_Height;

    // Color attachments
    std::vector<FrameBufferAttachmentSpecs> m_ColorAttachmentsSpecs = {};
    std::vector<uint32_t> m_ColorAttachments = {};

    // Depth attachment
    FrameBufferAttachmentSpecs m_DepthAttachmentSpecs = TextureFormat::None;
    uint32_t m_DepthAttachment = 0;
  };

}
