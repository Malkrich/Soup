#pragma once

#include "Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Soup
{

  class OpenGLContext : public GraphicsContext
  {
  public:
    OpenGLContext(GLFWwindow* window);

    virtual ~OpenGLContext() = default;

    virtual void Init() override;
    virtual void SwapBuffers() override;

  private:
    GLFWwindow* m_Window;
  };

}
