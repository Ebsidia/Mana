#pragma once

#include "Mana/Core/Layer.h"

#include "Mana/Events/ApplicationEvent.h"
#include "Mana/Events/KeyEvent.h"
#include "Mana/Events/MouseEvent.h"

namespace Mana {
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void onAttach() override;
        virtual void onDetach() override;

        virtual void onEvent(Event& event) override;

        void begin();
        void end();

        void SetThemeColors();

        void setBlockEvents(bool block) { m_blockEvents = block; }
    private:
        bool m_blockEvents = true;
        float m_time = 0.0f;

    };
}