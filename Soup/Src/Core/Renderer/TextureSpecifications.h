#pragma once

namespace Soup
{

  enum class TextureFormat
  {
    None = 0,

    // Color
    R8,
    RGB8,
    RGBA8,

    RGFloat16,
    RGFloat32,
    RGBFloat16,
    RGBFloat32,

    // Depth
    Depth24Sentil8,

    // Defaults
    RGB = RGB8,
    RGBA = RGBA8,
    Depth = Depth24Sentil8
  };

  enum class TextureFilter
  {
    Linear,
    Nearest,
    LinearMipmapLinear
  };

  enum class TextureWrapMode
  {
    Repeat,
    ClampToEdge
  };

}
