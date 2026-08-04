#include "ImGuiLayer.h"

#include "Core/Application.h"

#include "Platforms/ImGui/ImGui.h"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Soup
{

  ImGuiLayer::ImGuiLayer() {}

  void ImGuiLayer::OnAttach()
  {
    // IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // GLFW
    GLFWwindow* window = (GLFWwindow*)Application::GetInstance()->GetWindow()->GetWindowHandle();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // ImGuizmo
    ImGuizmo::AllowAxisFlip(false);
  }

  void ImGuiLayer::OnDetach()
  {
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
  }

  void ImGuiLayer::OnEvent(Event& e)
  {
    if (m_BlockEvents)
    {
      ImGuiIO& io = ImGui::GetIO();
      bool isHandle = false;
      isHandle |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
      isHandle |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
      e.SetHandled(isHandle);
    }
  }

  void ImGuiLayer::Begin()
  {
    // IMGUI TEST
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    ImGuiIO& io = ImGui::GetIO();
    const Application* app = Application::GetInstance();
    io.DisplaySize = ImVec2((float)app->GetWindow()->GetWidth(), (float)app->GetWindow()->GetHeight());

    float time = (float)glfwGetTime();
    io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
    m_Time = time;
  }

  void ImGuiLayer::End()
  {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      GLFWwindow* window = (GLFWwindow*)Application::GetInstance()->GetWindow()->GetWindowHandle();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(window);
    }
  }

}
