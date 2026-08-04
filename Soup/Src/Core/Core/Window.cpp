#include "Window.h"

#include "Core/Event/ApplicationEvent.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"

#include <GLFW/glfw3.h>

namespace Soup
{
  Window::Window(const WindowSettings& windowSettings)
  {
    Initialize(windowSettings);
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

  void Window::Initialize(const WindowSettings& windowSettings)
  {
    m_Settings = windowSettings;

    /* Initialize the library */
    if (!glfwInit())
    {
      SP_LOG_ERROR("glfwInit() failed !");
      return;
    }

    /* Create a windowed mode window and its OpenGL context */
    m_Window = glfwCreateWindow(m_Settings.Width, m_Settings.Height, m_Settings.Name.c_str(), NULL, NULL);

    if (!m_Window)
    {
      SP_LOG_ERROR("window creation failed !");
      glfwTerminate();
      return;
    }

    m_Context = GraphicsContext::Create(m_Window);
    m_Context->Init();

    // set window settings as user pointer
    glfwSetWindowUserPointer(m_Window, &m_Settings);

    // VSync
    SetVSync(windowSettings.VSync);

    glfwSetWindowCloseCallback(m_Window,
      [](GLFWwindow* window)
      {
        WindowSettings wSettings = *(WindowSettings*)glfwGetWindowUserPointer(window);
        WindowCloseEvent e;
        wSettings.CallbackFn(e);
      });

    glfwSetWindowSizeCallback(m_Window,
      [](GLFWwindow* window, int width, int height)
      {
        WindowSettings wSettings = *(WindowSettings*)glfwGetWindowUserPointer(window);

        WindowResizeEvent e(width, height);
        wSettings.CallbackFn(e);

        wSettings.Width = width;
        wSettings.Height = height;
      });

    glfwSetKeyCallback(m_Window,
      [](GLFWwindow* window, int key, int scancode, int action, int mods)
      {
        WindowSettings wSettings = *(WindowSettings*)glfwGetWindowUserPointer(window);

        switch (action)
        {
          case GLFW_PRESS:
          {
            KeyPressedEvent e(key);
            wSettings.CallbackFn(e);
            break;
          }
          case GLFW_RELEASE:
          {
            KeyReleasedEvent e(key);
            wSettings.CallbackFn(e);
            break;
          }
        }
      });

    glfwSetCursorPosCallback(m_Window,
      [](GLFWwindow* window, double x, double y)
      {
        WindowSettings wSettings = *(WindowSettings*)glfwGetWindowUserPointer(window);

        MouseMovedEvent e((unsigned int)x, (unsigned int)y);
        wSettings.CallbackFn(e);
      });

    glfwSetMouseButtonCallback(m_Window,
      [](GLFWwindow* window, int button, int action, int mods)
      {
        WindowSettings wSettings = *(WindowSettings*)glfwGetWindowUserPointer(window);

        switch (action)
        {
          case GLFW_PRESS:
          {
            MousePressedEvent e(button);
            wSettings.CallbackFn(e);
            break;
          }
          case GLFW_RELEASE:
          {
            MouseReleasedEvent e(button);
            wSettings.CallbackFn(e);
            break;
          }
        }
      });

    glfwSetScrollCallback(m_Window,
      [](GLFWwindow* window, double offsetX, double offsetY)
      {
        WindowSettings wSettings = *(WindowSettings*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent e(offsetX, offsetY);
        wSettings.CallbackFn(e);
      });
  }

  void Window::SetVSync(bool enable)
  {
    glfwSwapInterval(enable ? 1 : 0);
    m_Settings.VSync = enable;
  }
}
