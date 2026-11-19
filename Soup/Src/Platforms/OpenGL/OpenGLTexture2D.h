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

    static Ref<OpenGLTexture2D> Copy(
      const Texture2DSpecifications& specs, uint32_t textureSource, uint32_t width, uint32_t height);

    ~OpenGLTexture2D();

    void Bind(uint32_t slot = 0) const;

    uint32_t GetWidth() const override { return m_Width; }
    uint32_t GetHeight() const override { return m_Height; }

    uint32_t GetHandle() const override { return m_TextureID; }

    const Texture2DSpecifications& GetSpecifications() const override { return m_Specs; }

    void Resize(uint32_t width, uint32_t height) override;

    /**
     * Read the pixel at the given coordinates as integer. Pixel origin is at the bottom left.
     * Return 0 if the pixel coordinates are out of bounds or if the texture format is not valid.
     */
    int32_t GetPixelInteger(uint32_t x, uint32_t y) const override;

  private:
    void Invalidate(const void* data);

  private:
    uint32_t m_TextureID = 0;

    uint32_t m_Width = 0;
    uint32_t m_Height = 0;

    Texture2DSpecifications m_Specs;
  };

}
