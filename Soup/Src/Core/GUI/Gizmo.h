#pragma once

#include "Scene/Entity.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Soup
{

  class Gizmo
  {
  public:
    enum class ManipulationType
    {
      None = 0,
      Translate,
      Rotate
    };

  public:
    Gizmo();

    /**
     * Called every frame to update the interaction and rendering.
     */
    void OnRender(const glm::uvec2 windowPosition, const glm::uvec2& windowDimensions);

    /**
     * ImGui specific functions
     */
    void SetContext(ImGuiWindow* window, ImDrawList* drawList);

    /**
     * Set the camera to use for gizmo rendering.
     */
    void SetCamera(const Ref<Camera> camera) { m_Camera = camera; }

    /**
     * Set the entity to manipulate.
     */
    void SetManipulatedEntity(Entity entity);

    /**
     * Set which kind of manipulation the gizmo should do.
     */
    void SetManipulationType(ManipulationType type) { m_ManipulationType = type; }

    /**
     * Return true if the cursor is currently on the gizmo.
     */
    bool IsHovered() const;

  private:
    Entity m_ManipulatedEntity = Entity();
    Ref<Camera> m_Camera = nullptr;

    bool m_Visible = false;
    ManipulationType m_ManipulationType = ManipulationType::None;
  };

}
