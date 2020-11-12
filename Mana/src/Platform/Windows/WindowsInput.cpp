#include "mapch.h"
#include "Mana/Core/Input.h"
#include "Mana/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Mana {

    bool Input::isKeyPressed(int keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto state = glfwGetKey(window, keycode);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::isMouseButtonPressed(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto state = glfwGetMouseButton(window, button);

        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::getMousePos()
    {
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        double xPos, yPos;

        glfwGetCursorPos(window, &xPos, &yPos);

        return { (float)xPos, (float)yPos };
    }

    float Input::getMouseX()
    {
        auto[x, y] = getMousePos();

        return x;
    }

    float Input::getMouseY()
    {
        auto [x, y] = getMousePos();

        return y;
    }

    

}