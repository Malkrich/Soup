#pragma once

#include "Geometry/Mesh.h"
#include "Renderer/RendererMaterial.h"

#include <entt.hpp>
#include <glm/glm.hpp>

namespace Soup
{

  class Entity;

  class Scene
  {
  public:
    /**
     * Creates an entity with no component.
     */
    Entity CreateEntity();

    /**
     * Create an entity with a transform component
     */
    Entity CreatePositionableEntity();

    ///@{
    /**
     * Create an entity with a mesh component.
     * An initial position can be specified using overloaded functions.
     * A material component can also be attached by using overloaded functions.
     */
    Entity CreateMeshEntity(Ref<Mesh> mesh);
    Entity CreateMeshEntity(Ref<Mesh> mesh, const glm::vec3& position);
    Entity CreateMeshEntity(Ref<Mesh> mesh, const RendererMaterial& material);
    Entity CreateMeshEntity(Ref<Mesh> mesh, const glm::vec3& position, const RendererMaterial& material);
    ///@}

    ///@{
    /**
     * Create an entity with a light component.
     * An initial position and/or color can be specified using overloaded functions.
     */
    Entity CreateLightEntity();
    Entity CreateLightEntity(const glm::vec3& position);
    Entity CreateLightEntity(const glm::vec3& position, const glm::vec3& color);
    ///@}

    ///@{
    /**
     * Remove the entity associated to the given object or ID.
     */
    void RemoveEntity(uint32_t entityId);
    void RemoveEntity(Entity entity);
    ///@}

    /**
     * Return the entity from it's given id.
     * TODO: should take a UUID
     */
    Entity GetEntity(uint32_t entityId);

    /**
     * Call the functor to all entities that have the given components
     */
    template <typename... Components>
    void EntityView(std::function<void(Entity)> functor)
    {
      auto view = m_Registry.view<Components...>();
      for (auto id : view)
      {
        functor({ id, this });
      }
    }

    ///@{
    /**
     * Return the number of entity having the given components.
     */
    template <typename Components>
    size_t GetEntityCount()
    {
      return m_Registry.view<Components>().size();
    }
    template <typename... Components>
    size_t GetEntityCount()
    {
      return m_Registry.view<Components...>().size_hint();
    }
    ///@}

  private:
    entt::registry m_Registry;

    friend class Entity;
  };

}
