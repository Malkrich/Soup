#include "OpenGLTexture2D.h"

#include "OpenGLError.h"
#include "OpenGLTextureUtils.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Soup
{

  OpenGLTexture2D::OpenGLTexture2D(const Texture2DSpecifications& specs, void* data)
    : m_Specs(specs)
  {
    Invalidate(data);
  }

  Ref<OpenGLTexture2D> OpenGLTexture2D::Copy(const Texture2DSpecifications& specs, uint32_t textureSource)
  {
    Ref<OpenGLTexture2D> texture = CreateRef<OpenGLTexture2D>(specs);

    glCopyImageSubData(textureSource, GL_TEXTURE_2D, 0, 0, 0, 0, texture->m_TextureID, GL_TEXTURE_2D, 0, 0, 0, 0,
      specs.Width, specs.Height, 1);

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
    if (m_Specs.Width == width && m_Specs.Height == height)
    {
      return;
    }

    m_Specs.Width = width;
    m_Specs.Height = height;
    Invalidate(nullptr);
  }

  void OpenGLTexture2D::Invalidate(const void* data)
  {
    if (m_TextureID != 0)
    {
      glDeleteTextures(1, &m_TextureID);
      m_TextureID = 0;
    }

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    GLint internalFormat = OpenGLTextureUtils::TextureFormatToOpenGLTextureInternalFormat(m_Specs.Format);
    GLint format = OpenGLTextureUtils::TextureFormatToOpenGLTextureFormat(m_Specs.Format);
    GLint type = OpenGLTextureUtils::TextureFormatToOpenGLTextureType(m_Specs.Format);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Specs.Width, m_Specs.Height, 0, format, type, data);

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

  int32_t OpenGLTexture2D::GetPixelInteger(uint32_t x, uint32_t y) const
  {
    if (m_Specs.Format != TextureFormat::R8)
    {
      SP_LOG_ERROR("Could read pixel integer because texture format is not compatible. Returning 0.");
      return 0;
    }

    if (x < 0 || x >= m_Specs.Width || y < 0 || y >= m_Specs.Height)
    {
      SP_LOG_ERROR("Pixel coordinates are out of bonds. Returning 0.");
      return 0;
    }

    int32_t* pixelData = new int32_t[m_Specs.Width * m_Specs.Height];
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_INT, pixelData);

    int32_t pixelValue = pixelData[y * m_Specs.Width + x];
    delete[] pixelData;
    return pixelValue;
  }

}
