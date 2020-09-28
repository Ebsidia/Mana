#pragma once

#include "Mana/Layer.h"

#include "Mana/Events/ApplicationEvent.h"
#include "Mana/Events/KeyEvent.h"
#include "Mana/Events/MouseEvent.h"

namespace Mana {
    class MANA_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void onAttach() override;
        virtual void onDetach() override;
        virtual void onImGuiRender() override;
     
        void begin();
        void end();
    private:
        float m_time = 0.0f;

    };
}