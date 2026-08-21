#include "CameraController.h"

#include "Core/Input.h"
#include "Metric/World.h"
#include "Utils/MathUtils.h"

namespace Soup
{

  CameraController::CameraController(const CameraControllerSpecifications& specs)
    : m_Camera(specs.Width, specs.Height, specs.CameraPosition, specs.Pitch, specs.Yaw)
    , m_CurrentMousePosition(-1)
    , m_TargetPoint(specs.TargetPosition)
    , m_DistanceFromObject(specs.DistanceFromObject)
    , m_LaptopMode(specs.LaptopMode)
    , m_Width(specs.Width)
    , m_Height(specs.Height)
  {
  }

  void CameraController::Resize(float width, float height)
  {
    if (width != m_Width || height != m_Height)
    {
      m_Camera.SetViewportDimensions(width, height);
      m_Width = width;
      m_Height = height;
    }
  }

  void CameraController::SetTargetPoint(const glm::vec3& targetPoint)
  {
    // Update camera position
    glm::vec3 translation = targetPoint - m_TargetPoint;
    glm::vec3 newCameraPosition = m_Camera.GetPosition() + translation;
    m_Camera.SetPosition(newCameraPosition);

    // Set new target point
    m_TargetPoint = targetPoint;
  }

  void CameraController::OnUpdate()
  {
    glm::ivec2 newMousePosition = Input::GetMousePosition();
    if (Input::IsMouseButtonPressed(GetMovingButton()))
    {
      glm::ivec2 mouseOffset = newMousePosition - m_CurrentMousePosition;

      if (Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift))
        PanCamera(mouseOffset);
      else
        OrbitCamera(mouseOffset);
    }
    m_CurrentMousePosition = newMousePosition;
  }

  void CameraController::OnEvent(Event& e)
  {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(SP_BIND_EVENT_FUNCTION(CameraController::OnMouseScrolled));
  }

  bool CameraController::OnMouseScrolled(const MouseScrolledEvent& e)
  {
    m_DistanceFromObject -= (float)e.GetOffsetY() * s_ScrollSensitivity;
    glm::vec3 newPosition =
      MathUtils::SphericalToCartesian(m_Camera.GetPitch(), m_Camera.GetYaw(), m_DistanceFromObject);
    newPosition += m_TargetPoint;

    m_Camera.SetPosition(newPosition);

    return false;
  }

  void CameraController::OrbitCamera(const glm::ivec2& mouseOffset)
  {
    // The dot product gives a negative value when the camera is looking below the object
    float yawOffsetSense = glm::dot(World::GetUpVector(), m_Camera.GetUpVector()) > 0.0f ? 1.0f : -1.0f;
    float newPitch = m_Camera.GetPitch() - s_AngleSensitivity * mouseOffset.y;
    float newYaw = m_Camera.GetYaw() - s_AngleSensitivity * mouseOffset.x * yawOffsetSense;

    glm::vec3 newPosition = MathUtils::SphericalToCartesian(newPitch, newYaw, m_DistanceFromObject);
    newPosition += m_TargetPoint;

    m_Camera.SetPosition(newPosition);
    m_Camera.SetRotation(newPitch, newYaw);
  }

  void CameraController::PanCamera(const glm::ivec2& mouseOffset)
  {
    glm::vec3 translation = glm::vec3(0.0f);
    translation -= m_Camera.GetRightVector() * (float)mouseOffset.x * s_TranslationSensitivity;
    translation += m_Camera.GetUpVector() * (float)mouseOffset.y * s_TranslationSensitivity;

    glm::vec3 newPosition = m_Camera.GetPosition() + translation;
    m_TargetPoint += translation;

    m_Camera.SetPosition(newPosition);
  }

}
