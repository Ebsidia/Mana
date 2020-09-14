#include "mapch.h"
#include "Application.h"

#include "Mana/Events/ApplicationEvent.h"
#include "Log.h"


namespace Mana {

    Application::Application()
    {
        m_window = std::unique_ptr<Window>(Window::Create());
    }

    Application::~Application()
    {

    }

    void Application::run()
    {
        WindowResizeEvent e(1280, 720);
        MA_CORE_TRACE(e);

        while (m_running)
        {
            m_window->onUpdate();
        }
    }

}