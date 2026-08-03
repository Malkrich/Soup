#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Soup
{

  Camera::Camera(const glm::vec3& position, float pitch, float yaw)
    : m_Position(position)
    , m_Pitch(pitch)
    , m_Yaw(yaw)
  {
    UpdateViewMatrix();
  }

  void Camera::UpdateViewMatrix()
  {
    glm::quat orientation = glm::quat(glm::vec3(glm::radians(m_Pitch), glm::radians(m_Yaw), 0.0f));

    m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
    m_CameraVectorTransform = glm::mat3(m_ViewMatrix);
    m_ViewMatrix = glm::inverse(m_ViewMatrix);
  }

}
