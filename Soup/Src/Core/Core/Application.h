#pragma once

#include "Core/Event/ApplicationEvent.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Window.h"
#include "Gui/ImGuiLayer.h"

namespace Soup
{

  struct ApplicationSpecs
  {
    std::string Name = "Untitled";
    bool Headless = false;
  };

  class Application
  {
  public:
    Application(const ApplicationSpecs& specs);
    ~Application();

    static Application& GetInstance() { return *s_Instance; }
    /**
     * Return the window associated to the application instance.
     * This handle the graphics swap chain and events.
     */
    Ref<Window> GetWindow() const { return m_Window; }

    /**
     * Return the layer that is used for GUI updates.
     */
    Ref<ImGuiLayer> GetGuiLayer() const { return m_ImGuiLayer; }

    const std::string& GetName() const { return m_Specs.Name; }

    /**
     * Start the main event loop.
     */
    void Run();

    /**
     * Stop the event loop when the frame is finished.
     * This will yield to destroying all resources and quit the program.
     */
    void Quit();

    /**
     * Push a new active layer for the event loop.
     */
    template <typename T, typename... Args>
    void PushLayer(Args... args)
    {
      Ref<T> layer = CreateRef<T>(std::forward<Args>(args)...);
      m_LayerStack->PushLayer(layer);
    }

  private:
    void Initialize();

    /**
     * Triggered whenever an event is catched by the window.
     */
    void OnEvent(Event& e);

    /**
     * Called when the close button is clicked on the window.
     */
    bool OnWindowClose(const WindowCloseEvent& e);

  private:
    bool m_Running = true;
    float m_CurrentTime;

    ApplicationSpecs m_Specs;

    Scope<LayerStack> m_LayerStack = nullptr;
    Ref<ImGuiLayer> m_ImGuiLayer = nullptr;

    Ref<Window> m_Window = nullptr;

    inline static Application* s_Instance = nullptr;
  };

  Application* CreateApplication(int argc, char* argv[]);

}
