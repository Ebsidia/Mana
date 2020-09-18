#pragma once

/******************************************************************************
   File: Application.h

   Description:
       Application class that is intended to be inherited by Mana Applications


*******************************************************************************/

#include "core.h"

#include "Window.h"
#include "Mana/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"



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
    private:
        bool onWindowClosed(WindowClosedEvent& event);

    private:
        std::unique_ptr<Window> m_window;
        bool m_running = true;

        LayerStack m_layerStack;
    };

    //To be defined in Client Application
    Application* CreateApplication();
}