#pragma once

#include "Core/Event/MouseEvent.h"
#include "Core/KeyCodes.h"
#include "PerspectiveCamera.h"

namespace Soup
{

  struct CameraControllerSpecifications
  {
    // Control parameters
    glm::vec3 TargetPosition = { 0.0f, 0.0f, 0.0f };
    float DistanceFromObject = 2.0f;
    bool LaptopMode = false;

    // Camera parameters
    float Width, Height;
    glm::vec3 CameraPosition = { 0.0f, 0.0f, 1.0f };
    float Pitch = 0.0f, Yaw = 0.0f;
  };

  class CameraController
  {
  public:
    CameraController(const CameraControllerSpecifications& specs);

    const PerspectiveCamera& GetCamera() const { return m_Camera; }

    void Resize(float width, float height);

    void SetTargetPoint(const glm::vec3& targetPoint);

    void SetViewportHovered(bool hovered) { m_IsViewportHovered = hovered; }
    bool IsLaptopModeEnable() const { return m_LaptopMode; }
    void SetLaptopMode(bool laptopMode) { m_LaptopMode = laptopMode; }

    void OnEvent(Event& e);
    void OnUpdate();

  private:
    MouseButton GetMovingButton() const { return m_LaptopMode ? MouseButton::Left : MouseButton::Middle; }

    bool OnMouseScrolled(const MouseScrolledEvent& e);

    void PanCamera(const glm::ivec2& mouseOffset);
    void OrbitCamera(const glm::ivec2& mouseOffset);

  private:
    // Camera description
    PerspectiveCamera m_Camera;
    glm::vec3 m_TargetPoint;
    float m_DistanceFromObject;
    float m_Width, m_Height;

    // Control variables
    glm::ivec2 m_CurrentMousePosition = { 0, 0 };
    bool m_IsViewportHovered = true;
    bool m_IsCameraControlled = false;
    bool m_LaptopMode = false;

    // Constants
    inline static const float s_ScrollSensitivity = 0.02f;
    inline static const float s_AngleSensitivity = 0.15f;
    inline static const float s_TranslationSensitivity = 0.0003f;
  };

}
