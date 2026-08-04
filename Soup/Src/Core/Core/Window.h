#pragma once

#include "Event/Event.h"
#include "Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Soup
{

  class Window
  {
  public:
    struct WindowSettings
    {
    public:
      WindowSettings()
        : Name("Window")
        , Width(1280)
        , Height(720)
        , VSync(true)
      {
      }

      WindowSettings(const std::string& nameParam, uint32_t widthParam, uint32_t heightParam, bool vsync = true)
        : Name(nameParam)
        , Width(widthParam)
        , Height(heightParam)
        , VSync(vsync)
      {
      }

    public:
      std::string Name;

      uint32_t Width;
      uint32_t Height;

      bool VSync = true;

      EventDispatcher::EventCallbackFn CallbackFn;
    };

  public:
    Window(const WindowSettings& windowSettings);

    ~Window();

    uint32_t GetWidth() const { return m_Settings.Width; }
    uint32_t GetHeight() const { return m_Settings.Height; }

    void SetEventCallbackFunction(const EventDispatcher::EventCallbackFn& callbackFn)
    {
      m_Settings.CallbackFn = callbackFn;
    }

    void* GetWindowHandle() const { return (void*)m_Window; }

    bool IsVSync() const { return m_Settings.VSync; }
    void SetVSync(bool enable);

    void OnUpdate();

  private:
    void Initialize(const WindowSettings& windowSettings);

  private:
    GLFWwindow* m_Window;

    Scope<GraphicsContext> m_Context;

    WindowSettings m_Settings;
  };

}
