#include "Window.h"

#include "Core/Event/ApplicationEvent.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"

#include <GLFW/glfw3.h>

namespace Soup
{
  Window::Window(const WindowSpecifications& specs)
  {
    Initialize(specs);
  }

  Window::~Window()
  {
    glfwTerminate();
  }

  void Window::OnUpdate()
  {
    m_Context->SwapBuffers();

    /* Poll for and process events */
    glfwPollEvents();
  }

  void Window::Initialize(const WindowSpecifications& specs)
  {
    m_Specs = specs;

    /* Initialize the library */
    if (!glfwInit())
    {
      SP_LOG_ERROR("glfwInit() failed !");
      return;
    }

    /* Create a windowed mode window and its OpenGL context */
    m_Window = glfwCreateWindow(m_Specs.Width, m_Specs.Height, m_Specs.Name.c_str(), NULL, NULL);

    if (!m_Window)
    {
      SP_LOG_ERROR("window creation failed !");
      glfwTerminate();
      return;
    }

    m_Context = GraphicsContext::Create(m_Window);
    m_Context->Init();

    // set window settings as user pointer
    glfwSetWindowUserPointer(m_Window, &m_Specs);

    // VSync
    SetVSync(m_Specs.VSync);

    glfwSetWindowCloseCallback(m_Window,
      [](GLFWwindow* window)
      {
        WindowSpecifications* specs = (WindowSpecifications*)glfwGetWindowUserPointer(window);
        WindowCloseEvent e;
        specs->CallbackFn(e);
      });

    glfwSetWindowSizeCallback(m_Window,
      [](GLFWwindow* window, int width, int height)
      {
        WindowSpecifications* specs = (WindowSpecifications*)glfwGetWindowUserPointer(window);

        WindowResizeEvent e(width, height);
        specs->CallbackFn(e);

        specs->Width = width;
        specs->Height = height;
      });

    glfwSetKeyCallback(m_Window,
      [](GLFWwindow* window, int key, int scancode, int action, int mods)
      {
        WindowSpecifications* specs = (WindowSpecifications*)glfwGetWindowUserPointer(window);

        switch (action)
        {
          case GLFW_PRESS:
          {
            KeyPressedEvent e(key);
            specs->CallbackFn(e);
            break;
          }
          case GLFW_RELEASE:
          {
            KeyReleasedEvent e(key);
            specs->CallbackFn(e);
            break;
          }
        }
      });

    glfwSetCursorPosCallback(m_Window,
      [](GLFWwindow* window, double x, double y)
      {
        WindowSpecifications* specs = (WindowSpecifications*)glfwGetWindowUserPointer(window);

        MouseMovedEvent e((unsigned int)x, (unsigned int)y);
        specs->CallbackFn(e);
      });

    glfwSetMouseButtonCallback(m_Window,
      [](GLFWwindow* window, int button, int action, int mods)
      {
        WindowSpecifications* specs = (WindowSpecifications*)glfwGetWindowUserPointer(window);

        switch (action)
        {
          case GLFW_PRESS:
          {
            MousePressedEvent e(button);
            specs->CallbackFn(e);
            break;
          }
          case GLFW_RELEASE:
          {
            MouseReleasedEvent e(button);
            specs->CallbackFn(e);
            break;
          }
        }
      });

    glfwSetScrollCallback(m_Window,
      [](GLFWwindow* window, double offsetX, double offsetY)
      {
        WindowSpecifications* specs = (WindowSpecifications*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent e(offsetX, offsetY);
        specs->CallbackFn(e);
      });
  }

  void Window::SetVSync(bool enable)
  {
    glfwSwapInterval(enable ? 1 : 0);
    m_Specs.VSync = enable;
  }
}
