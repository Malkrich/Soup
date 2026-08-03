#pragma once

#include "Geometry/Mesh.h"
#include "Renderer/GpuMesh.h"
#include "Renderer/RendererMaterial.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Soup
{

  struct TransformComponent
  {
    glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
    glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& translation)
      : Translation(translation)
    {
    }

    glm::mat4 GetTransform() const
    {
      glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
      return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
    }
  };

  struct MeshComponent
  {
    Ref<GpuMesh> MeshData = nullptr;

    MeshComponent(Ref<Mesh> mesh) { MeshData = CreateRef<GpuMesh>(mesh); }
    MeshComponent() = default;
    MeshComponent(const MeshComponent&) = default;
  };

  struct LightComponent
  {
    glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
    // float Strength
    // float Radius

    LightComponent(const glm::vec3& color)
      : Color(color)
    {
    }
    LightComponent() = default;
    LightComponent(LightComponent&) = default;
  };

  struct MaterialComponent
  {
    RendererMaterial Material;

    MaterialComponent() = default;
    MaterialComponent(const RendererMaterial& material)
      : Material(material)
    {
    }
    MaterialComponent(const MaterialComponent&) = default;
  };

}
