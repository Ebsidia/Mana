#include "Application.h"

#include "Mana/Events/ApplicationEvent.h"
#include "Log.h"


namespace Mana {

    Application::Application()
    {
        
    }

    Application::~Application()
    {

    }

    void Application::run()
    {
        WindowResizeEvent e(1280, 720);
        MA_CORE_TRACE(e);

        while (true);
    }

}