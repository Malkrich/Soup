#pragma once

#include "Texture2D.h"
#include "TextureCubemap.h"
#include "TextureSpecifications.h"

#include <glm/glm.hpp>

namespace Soup
{

  struct FrameBufferAttachmentSpecs
  {
    TextureFormat Format = TextureFormat::None;
    TextureFilter MinFilter = TextureFilter::Linear;
    TextureFilter MagFilter = TextureFilter::Linear;
    uint32_t MipmapLevels = 1;
    Ref<Texture> ExistingAttachment = nullptr;

    // Clear
    bool ClearAttachemnt = false;
    glm::vec4 ClearColor = { 1.0f, 1.0f, 0.0f, 1.0f };

    // Misc
    bool Cubemap = false;
    bool Resizable = true;

    FrameBufferAttachmentSpecs() = default;
    FrameBufferAttachmentSpecs(TextureFormat format)
      : Format(format)
    {
    }
  };

  struct FrameBufferSpecifications
  {
    uint32_t Width = 0;
    uint32_t Height = 0;
    std::vector<FrameBufferAttachmentSpecs> Attachments = {};

    FrameBufferSpecifications() = default;
  };

  class FrameBuffer
  {
  public:
    static Ref<FrameBuffer> Create(const FrameBufferSpecifications& specs);

    virtual ~FrameBuffer() = default;

    virtual void Resize(uint32_t width, uint32_t height) = 0;
    void Resize(uint32_t dimensions) { Resize(dimensions, dimensions); }
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual const FrameBufferAttachmentSpecs& GetColorAttachmentSpecs(uint32_t attachmentIndex) const = 0;
    virtual const FrameBufferAttachmentSpecs& GetDepthAttachmentSpecs() const = 0;

    virtual void ClearAttachmentsIfNeeded() = 0;
    virtual int32_t ReadPixel(uint32_t attachmentIndex, uint32_t mouseX, uint32_t mouseY) const = 0;

    virtual uint32_t GetColorAttachment(uint32_t index) const = 0;
    virtual void SetColorAttachment(uint32_t index, Ref<Texture> texture) = 0;

    virtual void AttachCubemapFace(uint32_t attachmentIndex, CubemapFace face, uint32_t mipmapLevel = 0) = 0;
  };

}
