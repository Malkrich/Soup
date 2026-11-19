#pragma once

#include "Texture.h"
#include "TextureSpecifications.h"

namespace Soup
{

  struct Texture2DSpecifications
  {
    TextureFormat Format = TextureFormat::RGB;
    TextureWrapMode WrappingModeU = TextureWrapMode::Repeat;
    TextureWrapMode WrappingModeV = TextureWrapMode::Repeat;
    TextureFilter FilterMin = TextureFilter::Linear;
    TextureFilter FilterMag = TextureFilter::Linear;

    Texture2DSpecifications() = default;
  };

  class Texture2D : public Texture
  {
  public:
    static Ref<Texture2D> Create(const Texture2DSpecifications specs, const std::filesystem::path& filePath);
    static Ref<Texture2D> Create(const Texture2DSpecifications specs, uint32_t width, uint32_t height);
    static Ref<Texture2D> Create(const Texture2DSpecifications specs, uint32_t dimensions);

    static Ref<Texture2D> Copy(
      const Texture2DSpecifications& specs, uint32_t sourceTexture, uint32_t width, uint32_t height);
    static Ref<Texture2D> Copy(const Texture2DSpecifications& specs, uint32_t sourceTexture, uint32_t dimensions);

    virtual const Texture2DSpecifications& GetSpecifications() const = 0;

    /**
     * Resize the texture width and height.
     * Invalidate the current buffer handle to create a new one.
     */
    virtual void Resize(uint32_t width, uint32_t height) = 0;

    /**
     * Read the pixel at the given coordinates as integer.
     */
    virtual int32_t GetPixelInteger(uint32_t x, uint32_t y) const = 0;
  };

}
