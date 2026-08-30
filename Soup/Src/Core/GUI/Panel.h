#pragma once

#include "Core/Event/Event.h"
#include "StyleVariable.h"

namespace Soup
{

  class Panel
  {
  public:
    virtual ~Panel() = default;

    /**
     * Return true if the mouse cursor is on the panel.
     */
    bool IsHovered() const { return m_IsHovered; }

    /**
     * Return true if the mouse cursor clicked on the panel to focus it.
     */
    bool IsFocused() const { return m_IsFocused; }

    /**
     * Set the name for the panel.
     * This name is visible in the panel header.
     */
    void SetName(const std::string& name) { m_PanelName = name; }

    /**
     * Call to render the GUI on screen.
     */
    void OnGUIRender();

  protected:
    /**
     * Panel implementation of the GUI rendering.
     * This function is called by the Panel class after beginning a panel window context.
     */
    virtual void RenderPanelContent() = 0;

    /**
     * List of style variables that will be pushed before creating the window, then popped after the window is drawn.
     */
    StyleVariableList m_StyleVariables;

  private:
    std::string m_PanelName = "Unamed panel";

    bool m_IsHovered = false;
    bool m_IsFocused = false;
  };

}
