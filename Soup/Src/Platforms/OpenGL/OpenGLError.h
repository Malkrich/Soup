#pragma once

#include <glad/glad.h>

#ifdef SP_DEBUG
#define CHECK_GL_ERROR() Soup::CheckGLError(__FILE__, __LINE__)
#else
#define CHECK_GL_ERROR()
#endif

namespace Soup
{

  // For OpenGL operations
  // https://blog.nobel-joergensen.com/2013/01/29/debugging-opengl-using-glgeterror/
  void CheckGLError(const char* file, int32_t line);

  // For shaders
  void CheckGlslCompileError(const std::string& shaderName, const std::string& shaderSource, uint32_t shaderID);
  void CheckGlslLinkError(const std::string& shaderName, uint32_t shaderID);

  void OpenGLMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    const GLchar* message, const void* userParam);

}
