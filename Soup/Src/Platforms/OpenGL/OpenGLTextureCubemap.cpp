#include "OpenGLTextureCubemap.h"

#include "OpenGLError.h"
#include "OpenGLTextureUtils.h"
#include "Renderer/TextureUtils.h"

#include <glad/glad.h>

namespace Soup
{

  OpenGLTextureCubemap::OpenGLTextureCubemap(const TextureCubemapSpecifications& specs)
    : m_Specs(specs)
  {
    Invalidate();
  }

  Ref<OpenGLTextureCubemap> OpenGLTextureCubemap::Copy(const TextureCubemapSpecifications& specs, uint32_t textureID)
  {
    Ref<OpenGLTextureCubemap> cubemap = CreateRef<OpenGLTextureCubemap>(specs);

    for (uint32_t mipLevel = 0; mipLevel < specs.Levels; mipLevel++)
    {
      uint32_t width = TextureUtils::ComputeWidthOrHeightFromMipLevel(mipLevel, specs.Width);
      uint32_t height = TextureUtils::ComputeWidthOrHeightFromMipLevel(mipLevel, specs.Height);

      glCopyImageSubData(textureID, GL_TEXTURE_CUBE_MAP, mipLevel, 0, 0, 0, cubemap->m_TextureID, GL_TEXTURE_CUBE_MAP,
        mipLevel, 0, 0, 0, width, height, 6);
    }

    return cubemap;
  }

  OpenGLTextureCubemap::~OpenGLTextureCubemap()
  {
    glDeleteTextures(1, &m_TextureID);
  }

  void OpenGLTextureCubemap::Bind(uint32_t slot) const
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
  }

  void OpenGLTextureCubemap::RegenerateMipmaps()
  {
    SP_ASSERT(m_Specs.Levels > 1, "Cubemaps with level value of 1 can not have mipmaps.");

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  }

  void OpenGLTextureCubemap::Invalidate()
  {
    if (m_TextureID)
    {
      glDeleteTextures(1, &m_TextureID);
    }

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

    GLint internalFormat = OpenGLTextureUtils::TextureFormatToOpenGLTextureInternalFormat(m_Specs.Format);
    GLenum format = OpenGLTextureUtils::TextureFormatToOpenGLTextureFormat(m_Specs.Format);
    GLenum type = OpenGLTextureUtils::TextureFormatToOpenGLTextureType(m_Specs.Format);
    for (uint32_t faceIndex = 0; faceIndex < 6; faceIndex++)
    {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex, 0, internalFormat, m_Specs.Width, m_Specs.Height, 0,
        format, type, nullptr);
    }
    if (m_Specs.Levels > 1)
    {
      SP_ASSERT(
        m_Specs.MinFilter == TextureFilter::LinearMipmapLinear, "Mipmap texture must have LinearMipMapLinear filter!");
      glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    GLenum minFilter = OpenGLTextureUtils::TextureFilterToOpenGLFilter(m_Specs.MinFilter);
    GLenum magFilter = OpenGLTextureUtils::TextureFilterToOpenGLFilter(m_Specs.MagFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);

    CHECK_GL_ERROR();
  }

}
