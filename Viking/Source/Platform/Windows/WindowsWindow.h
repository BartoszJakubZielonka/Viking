//
// Created by batzi on 16.05.2021.
//

#ifndef VIKING_WINDOWSWINDOW_H
#define VIKING_WINDOWSWINDOW_H


#include "Viking/Core/Window.h"
#include "Viking/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Viking {
    class WindowsWindow final: public Window{
    public:
        WindowsWindow(const WindowProps);
        ~WindowsWindow() override;

        void onUpdate() override;

        [[nodiscard]] uint32_t getWidth() const override;

        [[nodiscard]] uint32_t getHeight() const override;

        void setEventCallback(const EventCallbackFn &callback) override;

        void setVSync(bool enabled) override;

        bool isVSync() const override;

        void *getNativeWindow() const override;

    private:
        void init(const WindowProps& props);
        void shutdown() const;

        GLFWwindow* mWindow;
        Scope<GraphicsContext> mContext;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData mData;
    };
}

#endif //VIKING_WINDOWSWINDOW_H
