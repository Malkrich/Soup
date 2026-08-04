#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Soup
{

  PerspectiveCamera::PerspectiveCamera(float width, float height, const glm::vec3& position, float pitch, float yaw,
    float fov, float nearClip, float farClip)
    : Camera(position, pitch, yaw)
    , m_ViewportWidth(width)
    , m_ViewportHeight(height)
    , m_AspectRatio(m_ViewportWidth / m_ViewportHeight)
    , m_Fov(fov)
    , m_NearClip(nearClip)
    , m_FarClip(farClip)
  {
    UpdateProjectionMatrix();
  }

  void PerspectiveCamera::SetViewportDimensions(float width, float height)
  {
    m_ViewportWidth = width;
    m_ViewportHeight = height;
    m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
    UpdateProjectionMatrix();
  }

  void PerspectiveCamera::UpdateProjectionMatrix()
  {
    m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
  }

}
