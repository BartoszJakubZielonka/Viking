//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_MOUSEEVENT_H
#define VIKING_MOUSEEVENT_H

#include "Hazel/Events/Event.h"
#include "Hazel/Core/MouseCodes.h"

namespace Viking {

    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(const float x, const float y): mMouseX(x), mMouseY(y) {}

        float getX() const { return mMouseX; }
        float getY() const { return mMouseY; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << mMouseX << ", " << mMouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float mMouseX, mMouseY;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(const float xOffset, const float yOffset)
                : mXOffset(xOffset), mYOffset(yOffset) {}

        float getXOffset() const { return m_XOffset; }
        float getYOffset() const { return m_YOffset; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float mXOffset, mYOffset;
    };

    class MouseButtonEvent : public Event
    {
    public:
        MouseCode getMouseButton() const { return mButton; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
    protected:
        MouseButtonEvent(const MouseCode button)
                : mButton(button) {}

        MouseCode mButton;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(const MouseCode button)
                : MouseButtonEvent(button) {}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << mButton;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(const MouseCode button)
                : MouseButtonEvent(button) {}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << mButton;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

}

#endif //VIKING_MOUSEEVENT_H
