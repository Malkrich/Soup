#include "Entity.h"

namespace Soup
{

  Entity::Entity(entt::entity handle, Scene* scene)
    : m_EntityHandle(handle)
    , m_Scene(scene)
  {
  }

}
