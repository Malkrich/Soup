#pragma once

namespace Soup
{

  class Asset;

  class TextureReader
  {
  public:
    /**
     * Import an image with uint8_t pixel format.
     */
    static Ref<Asset> ImportImageTexture(const std::filesystem::path& filePath);

    /**
     * Import an image with float pixel format.
     */
    static Ref<Asset> ImportHDRITexture(const std::filesystem::path& filePath);
  };

}
