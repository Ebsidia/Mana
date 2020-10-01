#include "mapch.h"
#include "Application.h"

#include "Mana/Events/ApplicationEvent.h"
#include "Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Mana/Input.h"

namespace Mana {

    Application* Application::s_instance = nullptr;

    Application::Application()
    {
        MA_CORE_ASSERT(!s_instance, "Appication already exists!");
        s_instance = this;

        m_window = std::unique_ptr<Window>(Window::Create());
        m_window->setEventCallback(MA_BIND_EVENT_FN(Application::onEvent));

        m_imguiLayer = new ImGuiLayer;
        pushOverlay(m_imguiLayer);
    }

    Application::~Application()
    {

    }

    void Application::run()
    {
       
        while (m_running)
        {
            glClearColor(0.33f, 0.0f, 0.66f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            for (Layer* layer : m_layerStack)
                layer->onUpdate();
            

            m_imguiLayer->begin();

            for (Layer* layer : m_layerStack)
                layer->onImGuiRender();

            m_imguiLayer->end();

            //auto [x, y] = Input::getMousPos();
            //MA_CORE_TRACE("{0}, {1}", x, y);
            
            m_window->onUpdate();
        }
    }
    
    void Application::onEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowClosedEvent>(MA_BIND_EVENT_FN(Application::onWindowClosed));

        //MA_CORE_TRACE("{0}", event);

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
        layer->onAttach();
    }

    void Application::pushOverlay(Layer* overlay)
    {
        m_layerStack.pushOverlay(overlay);
        overlay->onAttach();
    }

    bool Application::onWindowClosed(WindowClosedEvent& event)
    {
        MA_CORE_TRACE("{0}", event);
        m_running = false;

        return true;
    }
}