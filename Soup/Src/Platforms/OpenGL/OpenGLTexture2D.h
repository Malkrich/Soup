#pragma once

#include "Renderer/Texture2D.h"

namespace Soup
{

  class OpenGLTexture2D : public Texture2D
  {
  public:
    // TODO: should be handled by a TextureImporter class
    OpenGLTexture2D(const Texture2DSpecifications& specs, const std::filesystem::path& filePath);

    OpenGLTexture2D(const Texture2DSpecifications& specs, uint32_t width, uint32_t height, void* data = nullptr);

    static Ref<OpenGLTexture2D> copy(
      const Texture2DSpecifications& specs, uint32_t textureSource, uint32_t width, uint32_t height);

    virtual ~OpenGLTexture2D();

    void Bind(uint32_t slot = 0) const;

    virtual uint32_t GetWidth() const override { return m_Width; }
    virtual uint32_t GetHeight() const override { return m_Height; }

    virtual uint32_t GetHandle() const override { return m_TextureID; }

    virtual const Texture2DSpecifications& GetSpecifications() const override { return m_Specs; }

    virtual void Resize(uint32_t width, uint32_t height) override;

  private:
    void Invalidate(const void* data);

  private:
    uint32_t m_TextureID = 0;

    uint32_t m_Width = 0;
    uint32_t m_Height = 0;

    Texture2DSpecifications m_Specs;
  };

}
