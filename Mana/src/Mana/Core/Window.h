#pragma once
#include "Mana/Core/core.h"
#include "Mana/Events/Event.h"

//Abstract representation of a window

namespace Mana
{
    struct WindowProps
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = "Mana Engine",
            unsigned int width = 1280,
            unsigned int height = 720)
            : Title(title), Width(width), Height(height)
        {}
    };

    class MANA_API Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() {}

        virtual void onUpdate() = 0;

        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;

        virtual void setEventCallback(const EventCallbackFn& callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;

        virtual void* getNativeWindow() const = 0;

        //Window Create function that needs to be implemented on a per platform basis.
        static Window* Create(const WindowProps& props = WindowProps());
    };
}