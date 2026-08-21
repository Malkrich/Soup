#include "Application.h"

#include "Core/Time.h"
#include "IO/MeshImporter.h"
#include "Renderer/Renderer.h"

namespace Soup
{

  Application::Application(const ApplicationSpecs& specs)
    : m_Specs(specs)
  {
    SP_ASSERT(!s_Instance, "Application class is already instanciated !");
    s_Instance = this;

    m_LayerStack = CreateScope<LayerStack>();
    Initialize();
  }

  Application::~Application()
  {
    if (!m_Specs.Headless)
    {
      Renderer::Shutdown();
    }
    Renderer::Shutdown();
    MeshImporter::Shutdown();
    ApplicationLogger::Shutdown();
  }

  void Application::Initialize()
  {
    ApplicationLogger::Init();
    MeshImporter::Init();
    Renderer::Init(RendererAPI::API::OpenGL);

    if (!m_Specs.Headless)
    {
      // Window
      WindowSpecifications windowSpecs;
      windowSpecs.Name = m_Specs.Name;
      m_Window = CreateRef<Window>(windowSpecs);
      m_Window->SetEventCallbackFunction(SP_BIND_EVENT_FUNCTION(Application::OnEvent));

      Renderer::InitAPI();

      // GUI
      m_ImGuiLayer = CreateRef<ImGuiLayer>();
      m_LayerStack->PushOverlay(m_ImGuiLayer);
    }
  }

  void Application::Run()
  {
    while (m_Running)
    {
      float time = Time::GetTime();
      DeltaTime dt = time - m_CurrentTime;
      m_CurrentTime = time;

      for (auto layer : *m_LayerStack)
      {
        layer->OnUpdate(dt);
      }

      if (!m_Specs.Headless)
      {
        m_ImGuiLayer->Begin();
        for (auto layer : *m_LayerStack)
        {
          layer->OnGuiRender();
        }
        m_ImGuiLayer->End();

        m_Window->OnUpdate();
      }
    }
  }

  void Application::Quit()
  {
    m_Running = false;
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

  bool Application::OnWindowClose(const WindowCloseEvent&)
  {
    Quit();
    return true;
  }

}
