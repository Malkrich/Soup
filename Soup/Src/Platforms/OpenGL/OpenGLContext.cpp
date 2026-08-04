#include "OpenGLContext.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace Soup
{

  OpenGLContext::OpenGLContext(GLFWwindow* window)
    : m_Window(window)
  {
  }

  void OpenGLContext::Init()
  {
    /* Make the window's context current */
    glfwMakeContextCurrent(m_Window);

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    SP_ASSERT(status, "Failed to initialize Glad!");

    SP_LOG_INFO("Using OpenGL API version {}", (char*)glGetString(GL_VERSION));
    SP_LOG_INFO(" - Vendor: {}", (char*)glGetString(GL_VENDOR));
    SP_LOG_INFO(" - Device: {}", (char*)glGetString(GL_RENDERER));

    SP_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5),
      "Soup requires at least OpenGL version 4.5!");
  }

  void OpenGLContext::SwapBuffers()
  {
    /* Swap front and back buffers */
    glfwSwapBuffers(m_Window);
  }

}
