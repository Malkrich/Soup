#pragma once

#include "Metric/World.h"

#include <glm/glm.hpp>

namespace Soup
{

  enum class CameraType
  {
    None = 0,
    Parallel,
    Perspective
  };

  class Camera
  {
  public:
    Camera();
    Camera(const glm::vec3& position, float pitch, float yaw);

    /**
     * Return the type of the camera.
     * Camera subclasses must override this function to return the right camera type.
     */
    virtual CameraType GetType() const { return CameraType::None; }

    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    virtual const glm::mat4& GetProjectionMatrix() const = 0;
    virtual glm::mat4 GetViewProjectionMatrix() const = 0;

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
    glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

    glm::vec3 m_Position = glm::vec3(0.0f);
    // https://learnopengl.com/Getting-Started/Camera
    // In degrees
    float m_Pitch = 0.0f;
    float m_Yaw = 0.0f;
    glm::mat3 m_CameraVectorTransform = glm::mat3(1.0f);
  };

}
