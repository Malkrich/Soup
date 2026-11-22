#include "TextureReader.h"

#include "Renderer/Texture2D.h"
#include "Renderer/TextureSpecifications.h"

#include <stb_image.h>

namespace Soup
{

  Ref<Asset> TextureReader::ImportImageTexture(const std::filesystem::path& filePath)
  {
    int width = 0, height = 0, channels = 0;
    void* imageData = (void*)stbi_load(filePath.generic_string().c_str(), &width, &height, &channels, 0);
    if (!imageData)
    {
      SP_LOG_ERROR("Could not load texture at path: {}", filePath.generic_string());
      return nullptr;
    }

    Texture2DSpecifications specs;
    switch (channels)
    {
      case 3:
        specs.Format = TextureFormat::RGB;
        break;
      case 4:
        specs.Format = TextureFormat::RGBA;
        break;
    }

    specs.Width = width;
    specs.Height = height;

    Ref<Texture2D> texture = Texture2D::Create(specs, imageData);

    stbi_image_free(imageData);

    return texture;
  }

  Ref<Asset> TextureReader::ImportHDRITexture(const std::filesystem::path& filePath)
  {
    int width = 0, height = 0, channels = 0;
    void* imageData = (void*)stbi_loadf(filePath.generic_string().c_str(), &width, &height, nullptr, 3);

    if (!imageData)
    {
      SP_LOG_ERROR("Could not load hdri file at path: {}", filePath.generic_string());
      return nullptr;
    }

    Texture2DSpecifications specs;
    specs.Width = width;
    specs.Height = height;
    specs.Format = TextureFormat::RGBFloat32;
    specs.WrappingModeU = TextureWrapMode::ClampToEdge;
    specs.WrappingModeV = TextureWrapMode::ClampToEdge;
    Ref<Texture2D> texture = Texture2D::Create(specs, imageData);

    stbi_image_free(imageData);

    return texture;
  }

}
