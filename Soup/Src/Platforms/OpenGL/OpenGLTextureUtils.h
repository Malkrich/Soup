#pragma once

#include "Renderer/TextureSpecifications.h"

#include <glad/glad.h>

namespace Soup::OpenGLTextureUtils
{

  GLint TextureFormatToOpenGLTextureInternalFormat(TextureFormat format);

  GLenum TextureFormatToOpenGLTextureFormat(TextureFormat format);

  GLenum TextureFormatToOpenGLTextureType(TextureFormat format);

  GLint TextureWrapModeToOpenGLWrapMode(TextureWrapMode mode);

  GLint TextureFilterToOpenGLFilter(TextureFilter filter);

}
