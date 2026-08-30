#include "Gizmo.h"

#include "Scene/Components.h"

#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

namespace Soup
{

  namespace Utils
  {

    static ImGuizmo::OPERATION ManipulationTypeToImGuizmoOperation(Gizmo::ManipulationType manipulationType)
    {
      switch (manipulationType)
      {
        case Gizmo::ManipulationType::Translate:
          return ImGuizmo::TRANSLATE;
        case Gizmo::ManipulationType::Rotate:
          return ImGuizmo::ROTATE;
        default:
          SP_ASSERT(false, "Unknown manip type!");
          return ImGuizmo::TRANSLATE;
      }
    }
  }

  Gizmo::Gizmo()
  {
    ImGuizmo::SetOrthographic(false);
  }

  void Gizmo::OnRender(const glm::uvec2 windowPosition, const glm::uvec2& windowDimensions)
  {
    if (!m_ManipulatedEntity || !m_Camera || m_ManipulationType == ManipulationType::None)
      return;

    Soup::TransformComponent& transform = m_ManipulatedEntity.GetComponent<Soup::TransformComponent>();
    glm::mat4 transformMatrix = transform.GetTransform();

    ImGuizmo::SetRect(windowPosition.x, windowPosition.y, windowDimensions.x, windowDimensions.y);

    ImGuizmo::OPERATION gizmoOperation = Utils::ManipulationTypeToImGuizmoOperation(m_ManipulationType);
    ImGuizmo::Manipulate(glm::value_ptr(m_Camera->GetViewMatrix()), glm::value_ptr(m_Camera->GetProjectionMatrix()),
      gizmoOperation, ImGuizmo::LOCAL, glm::value_ptr(transformMatrix));

    glm::vec3 translation, rotation, scale;
    ImGuizmo::DecomposeMatrixToComponents(
      glm::value_ptr(transformMatrix), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));
    transform.Translation = translation;
    transform.Rotation = glm::radians(rotation);
  }

  void Gizmo::SetContext(ImGuiWindow* window, ImDrawList* drawList)
  {
    ImGuizmo::SetAlternativeWindow(window);
    ImGuizmo::SetDrawlist(drawList);
  }

  void Gizmo::SetManipulatedEntity(Entity entity)
  {
    if (entity && !entity.HasComponent<TransformComponent>())
    {
      SP_LOG_ERROR("Entity {} does not have a transform component, could not set it as the enity to manipulate.",
        (uint32_t)entity);
      return;
    }
    m_ManipulatedEntity = entity;
    m_Visible = (bool)entity;
  }

  bool Gizmo::IsHovered() const
  {
    return ImGuizmo::IsOver() && m_Visible;
  }

}
