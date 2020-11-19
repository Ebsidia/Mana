#pragma once
#include "Mana/Core/core.h"
#include "Mana/Events/Event.h"

//Abstract representation of a window

namespace Mana
{
    struct WindowProps
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;

        WindowProps(const std::string& title = "Mana Engine",
            uint32_t width = 1600,
            uint32_t height = 900)
            : Title(title), Width(width), Height(height)
        {}
    };

    class  Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() {}

        virtual void onUpdate() = 0;

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;

        virtual void setEventCallback(const EventCallbackFn& callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;

        virtual void* getNativeWindow() const = 0;

        //Window Create function that needs to be implemented on a per platform basis.
        static Window* Create(const WindowProps& props = WindowProps());
    };
}