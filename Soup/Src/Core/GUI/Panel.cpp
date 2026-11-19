#include "Panel.h"

#include <imgui.h>

namespace Soup
{

  void Panel::OnGUIRender()
  {
    m_StyleVariables.Push();
    ImGui::Begin(m_PanelName.c_str());

    m_IsFocused = ImGui::IsWindowFocused();
    m_IsHovered = ImGui::IsWindowHovered();

    RenderPanelContent();

    ImGui::End();
    m_StyleVariables.Pop();
  }

}
