#include "mapch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Mana {

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_windowHandle(windowHandle)
    {
        MA_CORE_ASSERT(windowHandle, "Window handle is null");
    }

    void OpenGLContext::init()
    {
        glfwMakeContextCurrent(m_windowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        MA_CORE_ASSERT(status, "Failed to initialize GLAD");

        MA_CORE_INFO("OpenGL Info:");
        MA_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        MA_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        MA_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

    }

    void OpenGLContext::swapBuffers()
    {
        glfwSwapBuffers(m_windowHandle);
    }

}