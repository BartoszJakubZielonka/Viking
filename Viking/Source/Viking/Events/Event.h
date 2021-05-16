//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_EVENT_H
#define VIKING_EVENT_H

#include <vipch.h>
#include <functional>

#include "Viking/Debug/Instrumentor.h"
#include "Viking/Core/Base.h"

namespace Viking {
    // Events in Viking are currently blocking, meaning when an event occurs it
    // immediately gets dispatched and must be dealt with right then an there.
    // For the future, a better strategy might be to buffer events in an event
    // bus and process them during the "event" part of the update stage.

    enum class EventType {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory {
        None = 0,
        EventCategoryApplication    = BIT(0),
        EventCategoryInput          = BIT(1),
        EventCategoryKeyboard       = BIT(2),
        EventCategoryMouse          = BIT(3),
        EventCategoryMouseButton    = BIT(4)
    };

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }\
								virtual EventType getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

    class Event {
    public:
        virtual ~Event() = default;

        bool handled{false};

        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        virtual int getCategoryFlags() const = 0;
        virtual std::string toString() const {
            return getName();
        }

        bool isInCategory(EventCategory category) {
            return getCategoryFlags() & category;
        }
    };

    class EventDispatcher {
    public:
        EventDispatcher(Event& event): mEvent(event) {}

        // F will be deduced by the compiler
        template<typename T, typename F>
        bool dispatch(const F& func) {
            if(mEvent.getEventType() == T::getStaticType()) {
                mEvent.handled |= func(static_cast<T&>(mEvent));
                return true;
            }
            return false;
        }

    private:
        Event& mEvent;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e) {
        return os << e.toString();
    }
}

#endif //VIKING_EVENT_H
