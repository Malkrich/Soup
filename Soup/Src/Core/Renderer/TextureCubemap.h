#pragma once

#include "Texture.h"
#include "TextureSpecifications.h"

namespace Soup
{

  enum class CubemapFace
  {
    PositiveX = 0,
    NegativeX = 1,
    PositiveY = 2,
    NegativeY = 3,
    PositiveZ = 4,
    NegativeZ = 5,
  };

  struct TextureCubemapSpecifications
  {
    uint32_t Width = 0, Height = 0;
    uint32_t Levels = 1;
    TextureFormat Format = TextureFormat::RGBFloat16;
    TextureFilter MinFilter = TextureFilter::Linear;
    TextureFilter MagFilter = TextureFilter::Linear;

    TextureCubemapSpecifications() = default;
  };

  class TextureCubemap : public Texture
  {
  public:
    static Ref<TextureCubemap> Create(const TextureCubemapSpecifications& specs);
    static Ref<TextureCubemap> Copy(const TextureCubemapSpecifications& specs, uint32_t textureID);

    virtual const TextureCubemapSpecifications& GetSpecifications() const = 0;

    virtual void RegenerateMipmaps() = 0;
  };

}
