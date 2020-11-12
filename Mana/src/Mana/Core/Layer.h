#pragma once


#include "Mana/Core/core.h"
#include "Mana/Core/TimeStep.h"
#include "Mana/Events/Event.h"

namespace Mana
{
    class  Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void onAttach() {}
        virtual void onDetach() {}
        virtual void onUpdate(TimeStep timeStep) {}
        virtual void onImGuiRender() {}
        virtual void onEvent(Event& event) {}
        
        inline const std::string& getName() const { return m_debugName; }
    protected:
        std::string m_debugName;

    };
}