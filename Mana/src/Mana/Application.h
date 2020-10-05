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

#include "ImGui/ImGuiLayer.h"

//temporary
#include "Mana/Render/Shader.h"
#include "Mana/Render/Buffer.h"
#include "Mana/Render/VertexArray.h"

#include "Mana/Render/OrthographicCamera.h"

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

        inline Window& getWindow() { return *m_window; }
        inline static Application& get() { return *s_instance; }

    private:
        bool onWindowClosed(WindowClosedEvent& event);

        std::unique_ptr<Window> m_window;
        ImGuiLayer* m_imguiLayer;
        bool m_running = true;

        LayerStack m_layerStack;
    private:
        static Application* s_instance;
    };

    //To be defined in Client Application
    Application* CreateApplication();
}