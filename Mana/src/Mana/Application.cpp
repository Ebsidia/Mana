#include "mapch.h"
#include "Application.h"

#include "Log.h"

#include "Mana/Render/Renderer.h"

#include "Mana/Input.h"


#include "Mana/KeyCodes.h"
#include "mana/MouseButtonCodes.h"

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
        //m_camera.setPosition({ 0.5f, 0.5f, 0.0f });
        //m_camera.setRotaion(270.0f);

        float x = 0.0f;
        float y = 0.0f;
        float r = 0.0f;


        while (m_running)
        {

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