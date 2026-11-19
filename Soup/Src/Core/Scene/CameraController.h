#pragma once

#include "Core/Event/MouseEvent.h"
#include "Core/MouseButtons.h"
#include "Renderer/Camera.h"

namespace Soup
{

  class CameraController
  {
  public:
    CameraController() = default;

    /**
     * Called whenever an event is triggered.
     */
    void OnEvent(Event& e);

    /**
     * Called every frame to update the camera position and orientation based on the interaction.
     */
    void OnUpdate();

    ///@{
    /**
     * Set/Get the camera to control.
     */
    void SetCamera(Ref<Camera> camera) { m_Camera = camera; }
    Ref<Camera> GetCamera() { return m_Camera; }
    ///@}

    /**
     * Resize the camera viewport
     */
    void Resize(uint32_t width, uint32_t height);

    /**
     * Set the point around which the camera will orbit.
     */
    void SetTargetPoint(const glm::vec3& targetPoint);

    /**
     * If set to true, the left clicked is used for interaction, otherwise, the middle clicked is used.
     */
    void SetLaptopMode(bool laptopMode) { m_LaptopMode = laptopMode; }

    /**
     * If set to true, clicking on the mouse button will effectively start the interaction. Otherwise, the interaction
     * will be blocked.
     */
    void SetInteractionPossible(bool possible) { m_InteractionPossible = possible; }

  private:
    /**
     * Return the button to use for interaction
     */
    MouseButton GetInteractionButton() const { return m_LaptopMode ? MouseButton::Left : MouseButton::Middle; }

    ///@{
    /**
     * Event specific callbacks.
     */
    bool OnMousePressed(const MousePressedEvent& event);
    bool OnMouseReleased(const MouseReleasedEvent& event);
    bool OnMouseScrolled(const MouseScrolledEvent& event);
    ///@}

    ///@{
    /**
     * Move or rotate the camera based on the given cursor offset.
     */
    void PanCamera(const glm::ivec2& cursorOffset);
    void OrbitCamera(const glm::ivec2& cursorOffset);
    ///@}

  private:
    Ref<Camera> m_Camera = nullptr;
    glm::vec3 m_TargetPoint = glm::vec3(0.0f);
    float m_DistanceFromObject = 1.0f;

    bool m_InteractionPossible = false;
    bool m_Interacting = false;
    glm::ivec2 m_CursorPosition = { 0, 0 };

    bool m_LaptopMode = false;

    inline static const float s_ScrollSensitivity = 0.02f;
    inline static const float s_AngleSensitivity = 0.15f;
    inline static const float s_TranslationSensitivity = 0.0003f;
  };

}
