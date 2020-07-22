#pragma once

#include "core.h"

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