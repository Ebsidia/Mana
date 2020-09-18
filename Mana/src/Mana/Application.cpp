#include "mapch.h"
#include "Application.h"

#include "Mana/Events/ApplicationEvent.h"
#include "Log.h"


namespace Mana {

#define  BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application()
    {
        m_window = std::unique_ptr<Window>(Window::Create());
        m_window->setEventCallback(BIND_EVENT_FN(onEvent));
    }

    Application::~Application()
    {

    }

    void Application::run()
    {
       
        while (m_running)
        {
            for (Layer* layer : m_layerStack)
            {
                layer->onUpdate();
            }

            m_window->onUpdate();
        }

        
    }

    void Application::onEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FN(onWindowClosed));

        MA_CORE_TRACE("{0}", event);

        for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
        {
            (*--it)->onEvent(event);
            if (event.handled)
                break;
        }
    }

    void Application::pushLayer(Layer* layer)
    {
        m_layerStack.pushLayer(layer);
    }

    void Application::pushOverlay(Layer* overlay)
    {
        m_layerStack.pushOverlay(overlay);
    }

    bool Application::onWindowClosed(WindowClosedEvent& event)
    {
        m_running = false;

        return true;
    }

}