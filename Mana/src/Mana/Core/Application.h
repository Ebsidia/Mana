#pragma once

/******************************************************************************
   File: Application.h

   Description:
       Application class that is intended to be inherited by Mana Applications


*******************************************************************************/

#include "core.h"

#include "Window.h"
#include "Mana/Core/LayerStack.h"
#include "Mana/Events/Event.h"
#include "Mana/Events/ApplicationEvent.h"

#include "Mana/Core/TimeStep.h"

#include "Mana/ImGui/ImGuiLayer.h"

namespace Mana {

    class MANA_API Application
    {
    public:
        Application();
        virtual ~Application();

        void run();

        void onEvent(Event& event);

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);

        inline Window& getWindow() { return *m_window; }
        inline static Application& get() { return *s_instance; }

    private:
        bool onWindowClosed(WindowClosedEvent& event);
        bool onWindowResize(WindowResizeEvent& event);
        bool onKeyPressed(KeyPressedEvent& event);

    private:
        std::unique_ptr<Window> m_window;
        ImGuiLayer* m_imguiLayer;
        bool m_running = true;
        bool m_minimized = false;

        LayerStack m_layerStack;

        float m_lastFrameTime = 0.0f;
    private:
        static Application* s_instance;
    };

    //To be defined in Client Application
    Application* CreateApplication();
}