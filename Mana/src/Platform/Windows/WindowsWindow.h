#pragma once

#include "Mana/Window.h"

#include "GLFW/glfw3.h"

namespace Mana
{
    class WindowsWindow : public Window 
    {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void onUpdate() override;

        inline unsigned int getWidth() const override { return m_data.Width; }
        inline unsigned int getHeight() const override { return m_data.Height; }

        inline void setEventCallback(const EventCallbackFn& callback) override { m_data.EventCallback = callback; }
        void setVSync(bool enabled) override;
        bool isVSync() const override;

        virtual void* getNativeWindow() const { return m_window; }
    private:
        virtual void init(const WindowProps& props);
        virtual void shutdown();

    private:
        GLFWwindow* m_window;

        //Struct of window data for passing to GLFW for event purposes
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_data;
    };

    

}