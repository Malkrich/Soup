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

    virtual void Resize(uint32_t width, uint32_t height) = 0;
  };

}
