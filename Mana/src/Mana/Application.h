#pragma once

/******************************************************************************
   File: Application.h

   Description:
       Application class that is intended to be inherited by Mana Applications


*******************************************************************************/

#include "core.h"
#include "Events/Event.h"

#include "Window.h"

namespace Mana {

    class MANA_API Application
    {
    public:
        Application();
        virtual ~Application();

        void run();
    private:
        std::unique_ptr<Window> m_window;
        bool m_running = true;
    };

    //To be defined in Client Application
    Application* CreateApplication();
}