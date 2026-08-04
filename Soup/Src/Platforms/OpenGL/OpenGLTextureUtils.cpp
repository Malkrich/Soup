#include "OpenGLTextureUtils.h"

#include <glm/glm.hpp>

namespace Soup::OpenGLTextureUtils
{

  GLint TextureFormatToOpenGLTextureInternalFormat(TextureFormat format)
  {
    switch (format)
    {
      case TextureFormat::RGFloat16:
        return GL_RG16F;
      case TextureFormat::RGFloat32:
        return GL_RG32F;
      case TextureFormat::RGBFloat16:
        return GL_RGB16F;
      case TextureFormat::RGBFloat32:
        return GL_RGB32F;

      case TextureFormat::R8:
        return GL_R32I;
      case TextureFormat::RGB8:
        return GL_RGB;
      case TextureFormat::RGBA8:
        return GL_RGBA;

      case TextureFormat::Depth24Sentil8:
        return GL_DEPTH24_STENCIL8;

      default:
        SP_ASSERT(false, "Unknown texture format!");
        return 0;
    }
  }

  GLenum TextureFormatToOpenGLTextureFormat(TextureFormat format)
  {
    switch (format)
    {
      case TextureFormat::R8:
        return GL_RED_INTEGER;

      case TextureFormat::RGFloat16:
      case TextureFormat::RGFloat32:
        return GL_RG;

      case TextureFormat::RGB8:
      case TextureFormat::RGBFloat16:
      case TextureFormat::RGBFloat32:
        return GL_RGB;

      case TextureFormat::RGBA8:
        return GL_RGBA;

      case TextureFormat::Depth24Sentil8:
        return GL_DEPTH_STENCIL;

      default:
        SP_ASSERT(false, "Unknown texture format!");
        return 0;
    }
  }

  GLenum TextureFormatToOpenGLTextureType(TextureFormat format)
  {
    switch (format)
    {
      case TextureFormat::RGFloat16:
      case TextureFormat::RGFloat32:
      case TextureFormat::RGBFloat16:
      case TextureFormat::RGBFloat32:
        return GL_FLOAT;

      case TextureFormat::R8:
      case TextureFormat::RGB8:
      case TextureFormat::RGBA8:
        return GL_UNSIGNED_BYTE;

      case TextureFormat::Depth24Sentil8:
        return GL_UNSIGNED_INT_24_8;

      default:
        SP_ASSERT(false, "Unknown texture format!");
        return 0;
    }
  }

  GLint TextureWrapModeToOpenGLWrapMode(TextureWrapMode mode)
  {
    switch (mode)
    {
      case TextureWrapMode::Repeat:
        return GL_REPEAT;
      case TextureWrapMode::ClampToEdge:
        return GL_CLAMP_TO_EDGE;
    }

    SP_ASSERT(false, "Unknown texture wrap mode!");
    return GL_REPEAT;
  }

  GLint TextureFilterToOpenGLFilter(TextureFilter filter)
  {
    switch (filter)
    {
      case TextureFilter::Linear:
        return GL_LINEAR;
      case TextureFilter::Nearest:
        return GL_NEAREST;
      case TextureFilter::LinearMipmapLinear:
        return GL_LINEAR_MIPMAP_LINEAR;
      default:
        SP_ASSERT(false, "Unknown texture filter!");
        return GL_LINEAR;
    }
  }

}
