#pragma once

#include "Asset/Asset.h"
#include "Texture.h"
#include "TextureSpecifications.h"

namespace Soup
{

  struct Texture2DSpecifications
  {
    uint32_t Width = 0;
    uint32_t Height = 0;
    TextureFormat Format = TextureFormat::RGB;
    TextureWrapMode WrappingModeU = TextureWrapMode::Repeat;
    TextureWrapMode WrappingModeV = TextureWrapMode::Repeat;
    TextureFilter FilterMin = TextureFilter::Linear;
    TextureFilter FilterMag = TextureFilter::Linear;

    Texture2DSpecifications() = default;
  };

  class Texture2D
    : public Texture
    , public Asset
  {
  public:
    static Ref<Texture2D> Create(const Texture2DSpecifications specs, void* data = nullptr);

    static Ref<Texture2D> Copy(const Texture2DSpecifications& specs, uint32_t sourceTexture);

    AssetType GetAssetType() const override { return AssetType::Texture2D; }

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
