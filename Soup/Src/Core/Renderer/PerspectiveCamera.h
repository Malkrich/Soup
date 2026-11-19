#pragma once

#include "Camera.h"

#include <glm/glm.hpp>

namespace Soup
{

  class PerspectiveCamera : public Camera
  {
  public:
    PerspectiveCamera();
    PerspectiveCamera(float width, float height, const glm::vec3& position, float pitch, float yaw, float fov = 45.0f,
      float nearClip = 0.01f, float farClip = 100.0f);

    CameraType GetType() const override { return CameraType::Perspective; }

    const glm::mat4& GetProjectionMatrix() const override { return m_ProjectionMatrix; }
    glm::mat4 GetViewProjectionMatrix() const override { return m_ProjectionMatrix * GetViewMatrix(); }

    void SetViewportDimensions(float width, float height);

  private:
    void UpdateProjectionMatrix();

  private:
    // Projection matrix parameters
    float m_ViewportWidth = 300;
    float m_ViewportHeight = 300;
    float m_AspectRatio = 1.0f;
    float m_Fov = 45.0f;
    float m_NearClip = 0.01f;
    float m_FarClip = 100.0f;

    glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
  };

}
