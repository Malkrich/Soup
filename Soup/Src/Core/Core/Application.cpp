#include "Application.h"

#include "Core/Layer/Layer.h"
#include "Core/Time.h"
#include "Renderer/Renderer.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

namespace Soup
{

  Application::Application()
  {
    SP_ASSERT(!s_Instance, "Application class is already instanciated !");
    s_Instance = this;

    Renderer::Init();

    // initialize Application
    Initialize();
  }

  Application::~Application()
  {
    Renderer::Shutdown();
  }

  void Application::Initialize()
  {
    // Window
    Window::WindowSettings windowSettings;
    windowSettings.Name = "Soup";
    m_Window = CreateRef<Window>(windowSettings);
    m_Window->SetEventCallbackFunction(SP_BIND_EVENT_FUNCTION(Application::OnEvent));

    // GUI
    m_ImGuiLayer = CreateRef<ImGuiLayer>();

    // Layers
    m_LayerStack = CreateScope<LayerStack>();
    m_LayerStack->PushOverlay(m_ImGuiLayer);

    Renderer::InitAPI();
  }

  void Application::Run()
  {
    while (m_Running)
    {
      float time = Time::GetTime();
      DeltaTime dt = time - m_CurrentTime;
      m_CurrentTime = time;

      for (auto layer : *m_LayerStack)
        layer->OnUpdate(dt);

      m_ImGuiLayer->Begin();
      for (auto layer : *m_LayerStack)
        layer->OnGuiRender();
      m_ImGuiLayer->End();

      m_Window->OnUpdate();
    }
  }

  void Application::OnEvent(Event& e)
  {
    EventDispatcher dispatcher(e);

    for (auto it = m_LayerStack->end(); it != m_LayerStack->begin();)
    {
      if (e.IsHandled())
        break;

      (*(--it))->OnEvent(e);
    }

    dispatcher.Dispatch<WindowCloseEvent>(SP_BIND_EVENT_FUNCTION(Application::OnWindowClose));
  }

  bool Application::OnWindowClose(const WindowCloseEvent& e)
  {
    m_Running = false;
    return true;
  }

}
