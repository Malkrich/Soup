#pragma once

#include "Metric/World.h"

#include <glm/glm.hpp>

namespace Soup
{

  class Camera
  {
  public:
    Camera(const glm::vec3& position, float pitch, float yaw);

    virtual glm::mat4 GetViewProjectionMatrix() const = 0;
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

    // Camera position
    const glm::vec3& GetPosition() const { return m_Position; }
    void SetPosition(const glm::vec3& position)
    {
      m_Position = position;
      UpdateViewMatrix();
    }

    // Camera orientation
    float GetPitch() const { return m_Pitch; }
    float GetYaw() const { return m_Yaw; }
    void SetRotation(float pitch, float yaw)
    {
      m_Pitch = pitch;
      m_Yaw = yaw;
      UpdateViewMatrix();
    }
    glm::vec3 GetUpVector() const { return m_CameraVectorTransform * World::GetUpVector(); }
    glm::vec3 GetRightVector() const { return m_CameraVectorTransform * World::GetXUnitVector(); }

  private:
    void UpdateViewMatrix();

  private:
    glm::mat4 m_ViewMatrix;

    glm::vec3 m_Position;
    // https://learnopengl.com/Getting-Started/Camera
    // In degrees
    float m_Pitch, m_Yaw;
    glm::mat3 m_CameraVectorTransform;
  };

}
