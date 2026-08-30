#include "PanelManager.h"

namespace Soup
{

  void PanelManager::OnGUIRender()
  {
    for (const auto& [_, panel] : m_Panels)
    {
      panel->OnGUIRender();
    }
  }

}
