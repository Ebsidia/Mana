#include "mapch.h"
#include "Application.h"

#include "Log.h"

#include "Mana/Render/Renderer.h"

#include "Mana/Core/Input.h"

#include <GLFW/glfw3.h>

#include "Mana/Core/KeyCodes.h"

namespace Mana {

    Application* Application::s_instance = nullptr;

    Application::Application()
    {
        MA_CORE_ASSERT(!s_instance, "Appication already exists!");
        s_instance = this;

        m_window = Scope<Window>(Window::Create());
        m_window->setEventCallback(MA_BIND_EVENT_FN(Application::onEvent));
        m_window->setVSync(true);

        Renderer::init();

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
            float time = (float)glfwGetTime();
            TimeStep timeStep = time - m_lastFrameTime;
            m_lastFrameTime = time;

            if (!m_minimized) 
            {
                for (Layer* layer : m_layerStack)
                    layer->onUpdate(timeStep);

                m_imguiLayer->begin();
                for (Layer* layer : m_layerStack)
                    layer->onImGuiRender();

                m_imguiLayer->end();
            }


            //auto [x, y] = Input::getMousPos();
            //MA_CORE_TRACE("{0}, {1}", x, y);
            
            m_window->onUpdate();
        }
    }
    
    void Application::onEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowClosedEvent>(MA_BIND_EVENT_FN(Application::onWindowClosed));
        dispatcher.Dispatch<WindowResizeEvent>(MA_BIND_EVENT_FN(Application::onWindowResize));
        dispatcher.Dispatch<KeyPressedEvent>(MA_BIND_EVENT_FN(Application::onKeyPressed));
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

    bool Application::onWindowResize(WindowResizeEvent& event)
    {
        if (event.getWidth() == 0 || event.getHeight() == 0)
        {
            m_minimized = true;
            return false;
        }

        m_minimized = false;

        Renderer::onWindowResize(event.getWidth(), event.getHeight());

        return false;
    }

    bool Application::onKeyPressed(KeyPressedEvent& event)
    {
        if (Input::isKeyPressed(MA_KEY_F1) && m_window->isVSync() == true)
        {
            m_window->setVSync(false);
            MA_CORE_INFO("VSync: {0}", m_window->isVSync());
        }
        else if (Input::isKeyPressed(MA_KEY_F1) && m_window->isVSync() == false)
        {
            m_window->setVSync(true);
            MA_CORE_INFO("VSync: {0}", m_window->isVSync());
        }

        return false;
    }
}