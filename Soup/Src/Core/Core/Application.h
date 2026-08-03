#pragma once

#include "Core/Event/ApplicationEvent.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Window.h"
#include "Gui/ImGuiLayer.h"

namespace Soup
{

  class Application
  {
  public:
    Application();

    ~Application();

    static const Application* GetInstance() { return s_Instance; }
    Ref<Window> GetWindow() const { return m_Window; }
    Ref<ImGuiLayer> GetGuiLayer() const { return m_ImGuiLayer; }

    void Run();

    template <typename T, typename... Args>
    void PushLayer(Args... args)
    {
      Ref<T> layer = CreateRef<T>(std::forward<Args>(args)...);
      m_LayerStack->PushLayer(layer);
    }

  private:
    void Initialize();

    void OnEvent(Event& e);

    bool OnWindowClose(const WindowCloseEvent& e);

  private:
    bool m_Running = true;

    Ref<Window> m_Window = nullptr;

    Scope<LayerStack> m_LayerStack = nullptr;

    Ref<ImGuiLayer> m_ImGuiLayer = nullptr;

    float m_CurrentTime;

    inline static Application* s_Instance = nullptr;
  };

  Application* CreateApplication();

}
