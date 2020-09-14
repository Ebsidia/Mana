#pragma once

/******************************************************************************
   File: Application.h

   Description:
       Application class that is intended to be inherited by Mana Applications


*******************************************************************************/

#include "core.h"
#include "Events/Event.h"

namespace Mana {

    class MANA_API Application
    {
    public:
        Application();
        virtual ~Application();

        void run();
    private:

    };

    //To be defined in Client Application
    Application* CreateApplication();
}