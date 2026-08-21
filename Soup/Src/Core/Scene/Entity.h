#pragma once

#include <Scene/Scene.h>

#include <entt.hpp>

namespace Soup
{

  class Entity
  {
  public:
    Entity() = default;

    template <typename T, typename... Args>
    T& AddComponent(Args&&... args)
    {
      SP_ASSERT(!HasComponent<T>(), "Component already exists!");
      return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
    }

    template <typename T>
    T& GetComponent()
    {
      SP_ASSERT(HasComponent<T>(), "Component does not exist!");
      return m_Scene->m_Registry.get<T>(m_EntityHandle);
    }

    template <typename T>
    bool HasComponent() const
    {
      return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
    }

    operator uint32_t() const { return (uint32_t)m_EntityHandle; }
    operator bool() const { return m_EntityHandle != entt::null && m_Scene != nullptr; }

  private:
    Entity(entt::entity handle, Scene* scene);
    operator entt::entity() const { return m_EntityHandle; }

  private:
    entt::entity m_EntityHandle = entt::null;
    Scene* m_Scene = nullptr;

    friend class Scene;
  };

}
