#pragma once

#include "entt.hpp"
#include "Mana/Core/TimeStep.h"

namespace Mana {

    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity createEntity(const std::string& name = std::string());
        void destroyEntity(Entity entity);


        void onUpdate(TimeStep dt);
        void onViewportResize(uint32_t width, uint32_t height);
    private:
        template<typename T>
        void onComponentAdded(Entity entity, T& component);
    private:
        entt::registry m_registry;
        uint32_t m_viewportWidth = 0;
        uint32_t m_viewportHeight = 0;

        friend class Entity;
        friend class SceneSerialization;
        friend class SceneHierarchyPanel;
    };
}