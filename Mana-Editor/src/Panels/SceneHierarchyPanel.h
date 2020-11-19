#pragma once

#include "Mana/Scene/Scene.h"
#include "Mana/Scene/Entity.h"

namespace Mana {

    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);

        void setContext(const Ref<Scene>& context);

        void onImGuiRender();

    private:
        void drawEntityNode(Entity entity);
        void drawComponents(Entity entity);
    private:
        Ref<Scene> m_context;
        Entity m_selection;
    };
}