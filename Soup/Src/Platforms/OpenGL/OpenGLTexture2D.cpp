#include "OpenGLTexture2D.h"

#include "OpenGLError.h"
#include "OpenGLTextureUtils.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Soup
{

  OpenGLTexture2D::OpenGLTexture2D(const Texture2DSpecifications& specs, const std::filesystem::path& filePath)
    : m_Specs(specs)
  {
    void* imageData = nullptr;
    if (std::filesystem::exists(filePath))
    {
      std::string strPath = filePath.string();
      int width, height;
      switch (m_Specs.Format)
      {
        case TextureFormat::RGB8:
        case TextureFormat::RGBA8:
          imageData = (void*)stbi_load(strPath.c_str(), &width, &height, nullptr, 0);
          break;
        case TextureFormat::RGBFloat16:
        case TextureFormat::RGBFloat32:
          imageData = (void*)stbi_loadf(strPath.c_str(), &width, &height, nullptr, 0);
          break;
        default:
          SP_ASSERT(false, "Unsupported pixel format!");
          break;
      }
      SP_ASSERT(imageData, "Unable to load image data from {}", filePath.string());

      m_Width = width;
      m_Height = height;
      Invalidate(imageData);

      stbi_image_free(imageData);
    }
  }

  OpenGLTexture2D::OpenGLTexture2D(const Texture2DSpecifications& specs, uint32_t width, uint32_t height, void* data)
    : m_Specs(specs)
    , m_Width(width)
    , m_Height(height)
  {
    Invalidate(data);
  }

  Ref<OpenGLTexture2D> OpenGLTexture2D::copy(
    const Texture2DSpecifications& specs, uint32_t textureSource, uint32_t width, uint32_t height)
  {
    Ref<OpenGLTexture2D> texture = CreateRef<OpenGLTexture2D>(specs, width, height);

    glCopyImageSubData(
      textureSource, GL_TEXTURE_2D, 0, 0, 0, 0, texture->m_TextureID, GL_TEXTURE_2D, 0, 0, 0, 0, width, height, 1);

    return texture;
  }

  OpenGLTexture2D::~OpenGLTexture2D()
  {
    glDeleteTextures(1, &m_TextureID);
  }

  void OpenGLTexture2D::Bind(uint32_t slot) const
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
  }

  void OpenGLTexture2D::Resize(uint32_t width, uint32_t height)
  {
    if (m_Width == width && m_Height == height)
    {
      return;
    }

    m_Width = width;
    m_Height = height;
    Invalidate(nullptr);
  }

  void OpenGLTexture2D::Invalidate(const void* data)
  {
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    GLint internalFormat = OpenGLTextureUtils::TextureFormatToOpenGLTextureInternalFormat(m_Specs.Format);
    GLint format = OpenGLTextureUtils::TextureFormatToOpenGLTextureFormat(m_Specs.Format);
    GLint type = OpenGLTextureUtils::TextureFormatToOpenGLTextureType(m_Specs.Format);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, type, data);

    GLint wrapSParam = OpenGLTextureUtils::TextureWrapModeToOpenGLWrapMode(m_Specs.WrappingModeU);
    GLint wrapTParam = OpenGLTextureUtils::TextureWrapModeToOpenGLWrapMode(m_Specs.WrappingModeV);
    GLint filterMinParam = OpenGLTextureUtils::TextureFilterToOpenGLFilter(m_Specs.FilterMin);
    GLint filterMagParam = OpenGLTextureUtils::TextureFilterToOpenGLFilter(m_Specs.FilterMag);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapSParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapTParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMinParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMagParam);

    CHECK_GL_ERROR();
  }

}
