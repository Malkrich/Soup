#pragma once

#include "Core/Event/Event.h"
#include "Panel.h"

namespace Soup
{

  class Panel;

  class PanelManager
  {
  public:
    PanelManager() = default;

    /**
     * Return the panel associated to the given template type.
     * If no panel found, return a nullptr.
     */
    template <typename T>
    Ref<T> GetPanel() const requires(std::is_base_of<Panel, T>::value)
    {
      const std::string& panelClassName = typeid(T).name();
      auto it = m_Panels.find(panelClassName);
      if (it == m_Panels.end())
      {
        return nullptr;
      }
      return std::dynamic_pointer_cast<T>(it->second);
    }

    /**
     * Register the panel given by the template type with the name.
     */
    template <typename T, typename... Args>
    void AddPanel(const std::string& name, Args... args) requires(std::is_base_of<Panel, T>::value)
    {
      const std::string& panelClassName = typeid(T).name();
      Ref<T> panel = CreateRef<T>(std::forward<Args>(args)...);
      panel->SetName(name);
      m_Panels[panelClassName] = panel;
    }

    /**
     * Render all the panels.
     */
    void OnGUIRender();

  private:
    std::map<std::string, Ref<Panel>> m_Panels;
  };

}
