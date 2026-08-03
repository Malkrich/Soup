#pragma once

#include <entt.hpp>

namespace Soup
{

  class Entity;
  class SceneRenderer;

  class SceneRegistry
  {
  public:
    Entity CreateEntity();
    void RemoveEntity(Entity e);

  private:
    entt::registry m_Registry;

    friend class Entity;
  };

}
