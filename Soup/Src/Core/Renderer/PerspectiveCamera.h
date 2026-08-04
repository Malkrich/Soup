#pragma once

#include "Camera.h"

#include <glm/glm.hpp>

namespace Soup
{

  class PerspectiveCamera : public Camera
  {
  public:
    PerspectiveCamera(float width, float height, const glm::vec3& position, float pitch, float yaw, float fov = 45.0f,
      float nearClip = 0.01f, float farClip = 100.0f);

    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    virtual glm::mat4 GetViewProjectionMatrix() const override { return m_ProjectionMatrix * GetViewMatrix(); }

    void SetViewportDimensions(float width, float height);

  private:
    void UpdateProjectionMatrix();

  private:
    // Projection matrix parameters
    float m_ViewportWidth, m_ViewportHeight, m_AspectRatio;
    float m_Fov, m_NearClip, m_FarClip;

    glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
  };

}
