#include "CameraController.h"

#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Metric/World.h"
#include "Renderer/PerspectiveCamera.h"
#include "Utils/MathUtils.h"

namespace Soup
{

  void CameraController::Resize(uint32_t width, uint32_t height)
  {
    if (m_Camera->GetType() == CameraType::Perspective)
    {
      std::dynamic_pointer_cast<PerspectiveCamera>(m_Camera)->SetViewportDimensions((float)width, (float)height);
    }
  }

  void CameraController::SetTargetPoint(const glm::vec3& targetPoint)
  {
    // Update camera position
    glm::vec3 translation = targetPoint - m_TargetPoint;
    glm::vec3 newCameraPosition = m_Camera->GetPosition() + translation;
    m_Camera->SetPosition(newCameraPosition);

    // Set new target point
    m_TargetPoint = targetPoint;
  }

  void CameraController::OnUpdate()
  {
    if (!m_Interacting)
      return;

    glm::ivec2 newCursorPosition = Input::GetCursorPosition();
    if (Input::IsMouseButtonPressed(GetInteractionButton()))
    {
      glm::ivec2 mouseOffset = newCursorPosition - m_CursorPosition;

      if (Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift))
        PanCamera(mouseOffset);
      else
        OrbitCamera(mouseOffset);
    }
    m_CursorPosition = newCursorPosition;
  }

  void CameraController::OnEvent(Event& e)
  {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MousePressedEvent>([&](const MousePressedEvent& event) { return OnMousePressed(event); });
    dispatcher.Dispatch<MouseReleasedEvent>([&](const MouseReleasedEvent& event) { return OnMouseReleased(event); });
    dispatcher.Dispatch<MouseScrolledEvent>([&](const MouseScrolledEvent& event) { return OnMouseScrolled(event); });
  }

  bool CameraController::OnMousePressed(const MousePressedEvent& event)
  {
    if (event.GetMouseButton() == GetInteractionButton() && m_InteractionPossible)
    {
      m_CursorPosition = Input::GetCursorPosition();
      m_Interacting = true;
    }
    return false;
  }

  bool CameraController::OnMouseReleased(const MouseReleasedEvent& event)
  {
    if (event.GetMouseButton() == GetInteractionButton())
      m_Interacting = false;

    return false;
  }

  bool CameraController::OnMouseScrolled(const MouseScrolledEvent& event)
  {
    m_DistanceFromObject -= (float)event.GetOffsetY() * s_ScrollSensitivity;
    glm::vec3 newPosition =
      MathUtils::SphericalToCartesian(m_Camera->GetPitch(), m_Camera->GetYaw(), m_DistanceFromObject);
    newPosition += m_TargetPoint;

    m_Camera->SetPosition(newPosition);

    return false;
  }

  void CameraController::OrbitCamera(const glm::ivec2& cursorOffset)
  {
    // The dot product gives a negative value when the camera is looking below the object
    float yawOffsetSense = glm::dot(World::GetUpVector(), m_Camera->GetUpVector()) > 0.0f ? 1.0f : -1.0f;
    float newPitch = m_Camera->GetPitch() - s_AngleSensitivity * cursorOffset.y;
    float newYaw = m_Camera->GetYaw() - s_AngleSensitivity * cursorOffset.x * yawOffsetSense;

    glm::vec3 newPosition = MathUtils::SphericalToCartesian(newPitch, newYaw, m_DistanceFromObject);
    newPosition += m_TargetPoint;

    m_Camera->SetPosition(newPosition);
    m_Camera->SetRotation(newPitch, newYaw);
  }

  void CameraController::PanCamera(const glm::ivec2& cursorOffset)
  {
    glm::vec3 translation = glm::vec3(0.0f);
    translation -= m_Camera->GetRightVector() * (float)cursorOffset.x * s_TranslationSensitivity;
    translation += m_Camera->GetUpVector() * (float)cursorOffset.y * s_TranslationSensitivity;

    glm::vec3 newPosition = m_Camera->GetPosition() + translation;
    m_TargetPoint += translation;

    m_Camera->SetPosition(newPosition);
  }

}
