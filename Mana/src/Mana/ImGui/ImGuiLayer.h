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

        void onAttach();
        void onDetach();
        void onUpdate();
        void onEvent(Event& event);
    private:
        bool onMouseButtonPressedEvent(MouseButtonPressedEvent& event);
        bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
        bool onMouseMoveEvent(MouseMovedEvent& event);
        bool onMouseScrolledEvent(MouseScrolledEvent& event);
        bool onKeyPressedEvent(KeyPressedEvent& event);
        bool onKeyReleasedEvent(KeyReleasedEvent& event);
        bool onKeyTypedEvent(KeyTypedEvent& event);
        bool onWindowResized(WindowResizeEvent& event);
    private:
        float m_time = 0.0f;

    };
}