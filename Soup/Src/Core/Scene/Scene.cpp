#include "Scene.h"

#include <Scene/Components.h>
#include <Scene/Entity.h>

#include <glm/glm.hpp>

namespace Soup
{

  Entity Scene::CreateEntity()
  {
    entt::entity entityId = m_Registry.create();
    return Entity(entityId, this);
  }

  Entity Scene::CreatePositionableEntity()
  {
    Entity entity = CreateEntity();
    entity.AddComponent<TransformComponent>();
    return entity;
  }

  Entity Scene::CreateMeshEntity(Ref<Mesh> mesh)
  {
    return CreateMeshEntity(mesh, glm::vec3(0.0f), RendererMaterial());
  }

  Entity Scene::CreateMeshEntity(Ref<Mesh> mesh, const glm::vec3& position)
  {
    return CreateMeshEntity(mesh, position, RendererMaterial());
  }

  Entity Scene::CreateMeshEntity(Ref<Mesh> mesh, const RendererMaterial& material)
  {
    return CreateMeshEntity(mesh, glm::vec3(0.0f), material);
  }

  Entity Scene::CreateMeshEntity(Ref<Mesh> mesh, const glm::vec3& position, const RendererMaterial& material)
  {
    Entity entity = CreatePositionableEntity();
    entity.GetComponent<TransformComponent>().Translation = position;

    entity.AddComponent<MeshComponent>(mesh);
    entity.AddComponent<MaterialComponent>(material);

    return entity;
  }

  Entity Scene::CreateLightEntity()
  {
    return CreateLightEntity(glm::vec3(0.0f), glm::vec3(1.0f));
  }

  Entity Scene::CreateLightEntity(const glm::vec3& position)
  {
    return CreateLightEntity(position, glm::vec3(1.0f));
  }

  Entity Scene::CreateLightEntity(const glm::vec3& position, const glm::vec3& color)
  {
    Entity entity = CreatePositionableEntity();
    entity.GetComponent<TransformComponent>().Translation = position;
    entity.AddComponent<LightComponent>(color);

    return entity;
  }

  void Scene::RemoveEntity(uint32_t entityId)
  {
    SP_ASSERT(m_Registry.valid((entt::entity)entityId), "Invalid entity to remove!");
    m_Registry.destroy((entt::entity)entityId);
  }

  void Scene::RemoveEntity(Entity entity)
  {
    size_t size = m_Registry.view<TransformComponent, LightComponent>().size_hint();

    RemoveEntity((uint32_t)entity);
  }

  Entity Scene::GetEntity(uint32_t entityId)
  {
    SP_ASSERT(m_Registry.valid((entt::entity)entityId), "Invalid entity to remove!");
    return Entity((entt::entity)entityId, this);
  }

}
