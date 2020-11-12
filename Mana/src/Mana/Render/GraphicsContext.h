#pragma once

namespace Mana {

    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;

        virtual void init() = 0;
        virtual void swapBuffers() = 0;
    private:
    };
}