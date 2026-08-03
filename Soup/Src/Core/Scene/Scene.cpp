#include "Scene.h"

#include "Components.h"
#include "Entity.h"

#include <glm/glm.hpp>

namespace Soup
{

  Entity SceneRegistry::CreateEntity()
  {
    entt::entity e = m_Registry.create();

    Entity entity = Entity(e, this);
    entity.AddComponent<TransformComponent>();

    return entity;
  }

  void SceneRegistry::RemoveEntity(Entity e)
  {
    SP_ASSERT(m_Registry.valid((entt::entity)e), "Unvalid entity to remove!");
    m_Registry.destroy((entt::entity)e);
  }

}
