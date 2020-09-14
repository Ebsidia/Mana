#pragma once

#include "Event.h"

namespace Mana
{
    class MANA_API MouseMovedEvent : public Event 
    {
        MouseMovedEvent(float x, float y)
            : m_mouseX(x), m_mouseY(y) {}

        inline float getMouseX() const { return m_mouseX; }
        inline float getMouseY() const { return m_mouseY; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float m_mouseX, m_mouseY;
    };

    class MANA_API MouseScrolledEvent : public Event 
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : m_xOffset(xOffset), m_yOffset(yOffset) {}

        inline float getXOffset() const { return m_xOffset; }
        inline float getYOffset() const { return m_yOffset; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << getXOffset() << ", " << getYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_xOffset, m_yOffset;
    };

    class MANA_API MouseButtonEvent : public Event 
    {
    public:
        inline int getMouseButton() const { return m_button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventGategoryInput)

    protected:
        MouseButtonEvent(int button)
            : m_button(button) {}

        int m_button;
    };

    class MANA_API MouseButtonPressedEvent : public MouseButtonEvent
    {
        MouseButtonPressedEvent(int button)
            :MouseButtonEvent(button){}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MANA_API MouseButtonReleasedEvent : public MouseButtonEvent
    {
        MouseButtonReleasedEvent(int button)
            : MouseButtonEvent(button) {}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
}