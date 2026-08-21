#include "OpenGLFrameBuffer.h"

#include "OpenGLError.h"
#include "OpenGLTextureUtils.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Soup
{

  namespace Utils
  {

    // ------------------------------------------------------------------
    // Texture 2D attachment
    // ------------------------------------------------------------------
    static void CreateTexture2DAttachment(
      uint32_t attachmentID, uint32_t width, uint32_t height, TextureFormat format, GLenum minFilter, GLenum magFilter)
    {
      SP_ASSERT(
        minFilter != GL_LINEAR_MIPMAP_LINEAR, "Mip map linear filtering not implemented for Texture2D attachments!");

      glBindTexture(GL_TEXTURE_2D, attachmentID);

      // no deals with multisampling yet
      GLint glInternalFormat = OpenGLTextureUtils::TextureFormatToOpenGLTextureInternalFormat(format);
      GLenum glFormat = OpenGLTextureUtils::TextureFormatToOpenGLTextureFormat(format);
      GLenum glType = OpenGLTextureUtils::TextureFormatToOpenGLTextureType(format);
      glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glType, nullptr);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    static void AttachColorTexture(uint32_t attachmentID, uint32_t attachmentIndex)
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIndex, GL_TEXTURE_2D, attachmentID, 0);
    }

    // ------------------------------------------------------------------
    // Cubemap attachment
    // ------------------------------------------------------------------
    static void CreateTextureCubemapAttachment(
      uint32_t attachmentID, uint32_t width, uint32_t height, TextureFormat format, GLint minFilter, GLint magFilter)
    {
      glBindTexture(GL_TEXTURE_CUBE_MAP, attachmentID);

      GLint glInternalFormat = OpenGLTextureUtils::TextureFormatToOpenGLTextureInternalFormat(format);
      GLenum glFormat = OpenGLTextureUtils::TextureFormatToOpenGLTextureFormat(format);
      GLenum glType = OpenGLTextureUtils::TextureFormatToOpenGLTextureType(format);
      for (uint32_t i = 0; i < 6; i++)
      {
        glTexImage2D(
          GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glInternalFormat, width, height, 0, glFormat, glType, nullptr);
      }

      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

      CHECK_GL_ERROR();
    }

    // ------------------------------------------------------------------
    // Cubemap attachment
    // ------------------------------------------------------------------
    static void CreateAttachment(uint32_t attachmentID, uint32_t width, uint32_t height, TextureFormat format,
      GLenum minFilter, GLenum magFilter, bool cubemap)
    {
      if (cubemap)
      {
        CreateTextureCubemapAttachment(attachmentID, width, height, format, minFilter, magFilter);
      }
      else
      {
        CreateTexture2DAttachment(attachmentID, width, height, format, minFilter, magFilter);
      }
    }

    static void AttachCubemapTexture(uint32_t attachmentID, uint32_t attachmentIndex)
    {
      // By default we attach postive X face which is index 0
      // We also attach mipmap level 0 by default
      glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIndex, GL_TEXTURE_CUBE_MAP_POSITIVE_X, attachmentID, 0);

      CHECK_GL_ERROR();
    }

    static void AttachDepthTexture(uint32_t attachmentID)
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, attachmentID, 0);
    }

    // ------------------------------------------------------------------
    // Misc
    // ------------------------------------------------------------------
    static bool IsDepthFormat(TextureFormat format)
    {
      switch (format)
      {
        case TextureFormat::Depth24Sentil8:
          return true;
        default:
          return false;
      }
    }

  }

  OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecifications& specs)
    : m_Width(specs.Width)
    , m_Height(specs.Height)
  {
    PopulateAttachments(specs.Attachments);

    glCreateFramebuffers(1, &m_BufferID);

    Invalidate();
  }

  OpenGLFrameBuffer::~OpenGLFrameBuffer()
  {
    for (size_t i = 0; i < m_ColorAttachments.size(); i++)
    {
      // Delete color attachment if it is not external
      if (!m_ColorAttachmentsSpecs[i].ExistingAttachment)
        glDeleteTextures(1, &m_ColorAttachments[i]);
    }

    // Delete depth attachment if it is not external
    if (!m_DepthAttachmentSpecs.ExistingAttachment)
      glDeleteTextures(1, &m_DepthAttachment);

    glDeleteFramebuffers(1, &m_BufferID);
  }

  void OpenGLFrameBuffer::Bind() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);
    glViewport(0, 0, m_Width, m_Height);
  }

  void OpenGLFrameBuffer::Unbind() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    CHECK_GL_ERROR();
  }

  void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
  {
    if (m_Width == width && m_Height == height)
    {
      return;
    }

    m_Width = width;
    m_Height = height;
    Invalidate();
  }

  uint32_t OpenGLFrameBuffer::GetColorAttachment(uint32_t index) const
  {
    SP_ASSERT(index < m_ColorAttachments.size(), "Color attachement index at {} is out of bound!", index);
    return m_ColorAttachments[index];
  }

  void OpenGLFrameBuffer::SetColorAttachment(uint32_t index, Ref<Texture> texture)
  {
    SP_ASSERT(index < m_ColorAttachmentsSpecs.size(), "Color attachment specs at index {} is out of bound!", index);
    SP_ASSERT(texture, "Invalid texture object!");

    FrameBufferAttachmentSpecs& specs = m_ColorAttachmentsSpecs[index];
    specs.ExistingAttachment = texture;
    UpdateSpecsFromTexture(specs);

    uint32_t newAttachmentId = texture->GetHandle();
    m_ColorAttachments[index] = newAttachmentId;

    // Attach color texture or cubemap texture
    glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);
    if (specs.Cubemap)
    {
      Utils::AttachCubemapTexture(newAttachmentId, index);
    }
    else
    {
      Utils::AttachColorTexture(newAttachmentId, index);
    }
  }

  void OpenGLFrameBuffer::ClearAttachmentsIfNeeded()
  {
    for (size_t i = 0; i < m_ColorAttachmentsSpecs.size(); i++)
    {
      const FrameBufferAttachmentSpecs& colorAttachment = m_ColorAttachmentsSpecs[i];
      if (colorAttachment.ClearAttachemnt)
      {
        switch (colorAttachment.Format)
        {
          case TextureFormat::R8:
          {
            int value = (int)colorAttachment.ClearColor.r;
            glClearTexImage(m_ColorAttachments[i], 0, GL_RED_INTEGER, GL_INT, &value);
            break;
          }
          case TextureFormat::RGBA8:
          {
            glClearTexImage(m_ColorAttachments[i], 0, GL_RGBA, GL_FLOAT, glm::value_ptr(colorAttachment.ClearColor));
            break;
          }
          case TextureFormat::RGBFloat32:
          {
            glm::vec3 color = glm::vec3(colorAttachment.ClearColor);
            glClearTexImage(m_ColorAttachments[i], 0, GL_RGB, GL_FLOAT, glm::value_ptr(color));
            break;
          }
          default:
            SP_ASSERT(false, "Clear color not supported for format {}", (uint32_t)colorAttachment.Format);
            break;
        }
      }
    }

    if (m_DepthAttachmentSpecs.ClearAttachemnt)
    {
      switch (m_DepthAttachmentSpecs.Format)
      {
        case TextureFormat::Depth24Sentil8:
        {
          float depthValue = 1.0f;
          glClearTexImage(m_DepthAttachment, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, &depthValue);
          break;
        }
        default:
          SP_ASSERT(false, "Unsupported depth texture format!");
          break;
      }
    }
  }

  int32_t OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, uint32_t mouseX, uint32_t mouseY) const
  {
    SP_ASSERT(attachmentIndex < m_ColorAttachmentsSpecs.size(), "Index {} for color attachment is out of bound!",
      attachmentIndex);

    Bind();

    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    int32_t data;
    glReadPixels(mouseX, mouseY, 1, 1, GL_RED_INTEGER, GL_INT, &data);
    CHECK_GL_ERROR();

    Unbind();

    return data;
  }

  void OpenGLFrameBuffer::AttachCubemapFace(uint32_t attachmentIndex, CubemapFace face, uint32_t mipmapLevel)
  {
    SP_ASSERT(m_ColorAttachmentsSpecs[attachmentIndex].Cubemap, "Attachment index {} is not a cubemap attachment!",
      attachmentIndex);

    uint32_t faceIndex = (uint32_t)face;
    uint32_t attachmentID = m_ColorAttachments[attachmentIndex];
    glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentIndex,
      GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex, attachmentID, mipmapLevel);
  }

  void OpenGLFrameBuffer::Invalidate()
  {
    if (m_BufferID)
    {
      for (size_t i = 0; i < m_ColorAttachments.size(); i++)
      {
        const FrameBufferAttachmentSpecs& attachment = m_ColorAttachmentsSpecs[i];
        if (attachment.Resizable && !attachment.ExistingAttachment)
        {
          glDeleteTextures(1, &m_ColorAttachments[i]);
          m_ColorAttachments[i] = 0;
        }
      }
      if (m_DepthAttachmentSpecs.Resizable && !m_DepthAttachmentSpecs.ExistingAttachment)
      {
        glDeleteTextures(1, &m_DepthAttachment);
        m_DepthAttachment = 0;
      }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);

    // Color attachments
    if (!m_ColorAttachmentsSpecs.empty())
    {
      m_ColorAttachments.resize(m_ColorAttachmentsSpecs.size());

      for (size_t i = 0; i < m_ColorAttachmentsSpecs.size(); i++)
      {
        const FrameBufferAttachmentSpecs& colorAttachmentSpecs = m_ColorAttachmentsSpecs[i];
        uint32_t& attachmentID = m_ColorAttachments[i];

        Ref<Texture> existingAttachment = colorAttachmentSpecs.ExistingAttachment;

        if (existingAttachment)
        {
          if (colorAttachmentSpecs.Resizable && !colorAttachmentSpecs.Cubemap)
          {
            std::dynamic_pointer_cast<Texture2D>(existingAttachment)->Resize(m_Width, m_Height);
          }

          attachmentID = existingAttachment->GetHandle();
        }
        else if (colorAttachmentSpecs.Resizable)
        {
          glGenTextures(1, &attachmentID);

          GLenum minFilter = OpenGLTextureUtils::TextureFilterToOpenGLFilter(colorAttachmentSpecs.MinFilter);
          GLenum magFilter = OpenGLTextureUtils::TextureFilterToOpenGLFilter(colorAttachmentSpecs.MagFilter);
          bool cubemap = colorAttachmentSpecs.Cubemap;
          Utils::CreateAttachment(
            attachmentID, m_Width, m_Height, colorAttachmentSpecs.Format, minFilter, magFilter, cubemap);
        }

        // Attach color texture or cubemap texture
        if (colorAttachmentSpecs.Cubemap)
        {
          Utils::AttachCubemapTexture(attachmentID, i);
        }
        else
        {
          Utils::AttachColorTexture(attachmentID, i);
        }

        CHECK_GL_ERROR();
      }
    }

    // Depth attachment
    if (m_DepthAttachmentSpecs.Format != TextureFormat::None)
    {
      SP_ASSERT(!m_DepthAttachmentSpecs.Cubemap, "Cubemap depth attachment not supported!");

      if (m_DepthAttachmentSpecs.ExistingAttachment)
      {
        if (m_DepthAttachmentSpecs.Resizable && m_DepthAttachment)
        {
          std::dynamic_pointer_cast<Texture2D>(m_DepthAttachmentSpecs.ExistingAttachment)->Resize(m_Width, m_Height);
        }

        m_DepthAttachment = m_DepthAttachmentSpecs.ExistingAttachment->GetHandle();
      }
      else
      {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);

        GLenum minFilter = OpenGLTextureUtils::TextureFilterToOpenGLFilter(m_DepthAttachmentSpecs.MinFilter);
        GLenum magFilter = OpenGLTextureUtils::TextureFilterToOpenGLFilter(m_DepthAttachmentSpecs.MagFilter);

        Utils::CreateAttachment(
          m_DepthAttachment, m_Width, m_Height, m_DepthAttachmentSpecs.Format, minFilter, magFilter, false);
      }

      Utils::AttachDepthTexture(m_DepthAttachment);
    }

    if (!m_ColorAttachments.empty())
    {
      // TODO: review verification
      // and memory managment of attachmentIndices
      static std::array<GLenum, 4> attachmentIndices = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
      SP_ASSERT(m_ColorAttachments.size() <= attachmentIndices.size(),
        "Soup does not support more than {} color attachments!", attachmentIndices.size());
      glDrawBuffers(m_ColorAttachments.size(), attachmentIndices.data());
      CHECK_GL_ERROR();
    }
    else
    {
      glDrawBuffer(GL_NONE);
    }

    SP_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer is incomplete !");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void OpenGLFrameBuffer::PopulateAttachments(const std::vector<FrameBufferAttachmentSpecs>& attachments)
  {
    for (const auto& attachment : attachments)
    {
      if (Utils::IsDepthFormat(attachment.Format))
      {
        m_DepthAttachmentSpecs = attachment;
      }
      else
      {
        FrameBufferAttachmentSpecs& colorAttachment = m_ColorAttachmentsSpecs.emplace_back(attachment);

        if (attachment.ExistingAttachment)
          UpdateSpecsFromTexture(colorAttachment);
      }
    }
  }

  void OpenGLFrameBuffer::UpdateSpecsFromTexture(FrameBufferAttachmentSpecs& attachmentSpecs)
  {
    Ref<Texture> texture = attachmentSpecs.ExistingAttachment;

    if (attachmentSpecs.Cubemap)
    {
      Ref<TextureCubemap> textureCube = std::dynamic_pointer_cast<TextureCubemap>(texture);
      SP_ASSERT(textureCube, "Existing attachment is not a TextureCubemap");

      const TextureCubemapSpecifications& specs = textureCube->GetSpecifications();
      attachmentSpecs.Format = specs.Format;
      attachmentSpecs.MinFilter = specs.MinFilter;
      attachmentSpecs.MagFilter = specs.MagFilter;
      attachmentSpecs.MipmapLevels = specs.Levels;
    }
    else
    {
      Ref<Texture2D> texture2D = std::dynamic_pointer_cast<Texture2D>(texture);
      SP_ASSERT(texture2D, "Existing attachment is not a Texture2D");

      const Texture2DSpecifications& specs = texture2D->GetSpecifications();
      attachmentSpecs.Format = specs.Format;
      attachmentSpecs.MinFilter = specs.FilterMin;
      attachmentSpecs.MagFilter = specs.FilterMag;
    }
  }

}
