//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_WINDOW_H
#define VIKING_WINDOW_H

#include <sstream>
#include "Viking/Core/Base.h"
#include "Viking/Events//Event.h"

namespace Viking {
    struct WindowProps {
        std::string Title;
        uint32_t Width;
        uint32_t Height;

        explicit WindowProps(const std::string& title = "Viking Engine", uint32_t width = 1600, uint32_t height = 900): Title(title), Width(width), Height(height) {}
    };

    //Interface representing a desktop system based Window.
    class Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void onUpdate() = 0;

        [[nodiscard]] virtual uint32_t getWidth() const = 0;
        [[nodiscard]] virtual uint32_t getHeight() const = 0;

        //Window attributes
        virtual void setEventCallback(const EventCallbackFn& callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        [[nodiscard]] virtual bool isVSync() const = 0;

        [[nodiscard]] virtual void* getNativeWindow() const = 0;

        static Scope<Window> create(const WindowProps props = WindowProps());
    };
}

#endif //VIKING_WINDOW_H
