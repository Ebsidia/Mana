#pragma once

#include "Mana/Core/core.h"
#include "Mana/Core/KeyCodes.h"
#include "Mana/Core/MouseButtonCodes.h"

namespace Mana {
    class  Input 
    {
    public:

        static bool isKeyPressed(int keycode);

        static bool isMouseButtonPressed(int button);
        static std::pair<float, float> getMousePos();
        static float getMouseX();
        static float getMouseY();
    };
}