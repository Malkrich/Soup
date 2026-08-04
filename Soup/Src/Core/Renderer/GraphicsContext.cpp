#include "GraphicsContext.h"

#include "Renderer.h"

#include "Platforms/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>

namespace Soup
{

  Scope<GraphicsContext> GraphicsContext::Create(void* window)
  {
    switch (Renderer::GetCurrentAPI())
    {
      case RendererAPI::API::None:
        SP_ASSERT(false, "Soup does not support RendererAPI::None!");
        return nullptr;
      case RendererAPI::API::OpenGL:
        return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
    }

    SP_ASSERT(false, "Unknown API!");
    return nullptr;
  }

}
