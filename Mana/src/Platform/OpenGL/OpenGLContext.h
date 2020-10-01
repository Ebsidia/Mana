#pragma once

#include "Mana/Render/GraphicsContext.h"

struct GLFWwindow;

namespace Mana {

    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);

        virtual void init() override;
        virtual void swapBuffers() override;

    private:
        GLFWwindow* m_windowHandle;
    };
}