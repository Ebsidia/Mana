#pragma once

#include "Entity.h"

namespace Mana {

    class ScriptableEntity
    {
    public:
        virtual ~ScriptableEntity() {}

        template<typename T>
        T& getComponent()
        {
            return m_entity.getComponent<T>();
        }
    protected:
        virtual void onCreate() {}
        virtual void onDestroy() {}
        virtual void onUpdate(TimeStep dt) {}
    private:
        Entity m_entity;

        friend class Scene;
    };
}