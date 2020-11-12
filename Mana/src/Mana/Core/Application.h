#pragma once

/******************************************************************************
   File: Application.h

   Description:
       Application class that is intended to be inherited by Mana Applications


*******************************************************************************/

#include "core.h"

#include "Window.h"
#include "Mana/Core/LayerStack.h"
#include "Mana/Events/Event.h"
#include "Mana/Events/ApplicationEvent.h"

#include "Mana/Core/TimeStep.h"

#include "Mana/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Mana {

    class  Application
    {
    public:
        Application(const std::string& name = "Mana Engine");
        virtual ~Application();

        void onEvent(Event& event);

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);

        inline Window& getWindow() { return *m_window; }

        void close();

        ImGuiLayer* getImGuiLayer() {return m_imguiLayer;}

        inline static Application& get() { return *s_instance; }

    private:
        void run();

        bool onWindowClosed(WindowClosedEvent& event);
        bool onWindowResize(WindowResizeEvent& event);
        bool onKeyPressed(KeyPressedEvent& event);

    private:
        std::unique_ptr<Window> m_window;
        ImGuiLayer* m_imguiLayer;
        bool m_running = true;
        bool m_minimized = false;

        LayerStack m_layerStack;

        float m_lastFrameTime = 0.0f;
    private:
        static Application* s_instance;
        friend int ::main(int argc, char** argv);
    };

    //To be defined in Client Application
    Application* CreateApplication();
}