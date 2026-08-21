#pragma once

#include "Event/Event.h"
#include "Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Soup
{

  struct WindowSpecifications
  {
    WindowSpecifications()
      : Name("Window")
      , Width(1280)
      , Height(720)
      , VSync(true)
    {
    }

    WindowSpecifications(const std::string& nameParam, uint32_t widthParam, uint32_t heightParam, bool vsync = true)
      : Name(nameParam)
      , Width(widthParam)
      , Height(heightParam)
      , VSync(vsync)
    {
    }

    std::string Name;

    uint32_t Width;
    uint32_t Height;

    bool VSync = true;

    EventDispatcher::EventCallbackFn CallbackFn;
  };

  class Window
  {
  public:
  public:
    Window(const WindowSpecifications& windowSettings);

    ~Window();

    uint32_t GetWidth() const { return m_Specs.Width; }
    uint32_t GetHeight() const { return m_Specs.Height; }

    void SetEventCallbackFunction(const EventDispatcher::EventCallbackFn& callbackFn)
    {
      m_Specs.CallbackFn = callbackFn;
    }

    void* GetWindowHandle() const { return (void*)m_Window; }

    bool IsVSync() const { return m_Specs.VSync; }
    void SetVSync(bool enable);

    void OnUpdate();

  private:
    void Initialize(const WindowSpecifications& windowSettings);

  private:
    GLFWwindow* m_Window;

    Scope<GraphicsContext> m_Context;

    WindowSpecifications m_Specs;
  };

}
