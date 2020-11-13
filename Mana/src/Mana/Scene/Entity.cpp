#include "mapch.h"
#include "Entity.h"

namespace Mana {

    Entity::Entity(entt::entity handle, Scene* scene)
        : m_entityHandle(handle), m_scene(scene)
    {

    }

}