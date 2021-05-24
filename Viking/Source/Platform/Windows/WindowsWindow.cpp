//
// Created by batzi on 16.05.2021.
//
#include "vipch.h"
#include "Platform/Windows/WindowsWindow.h"

#include "Viking/Core/Input.h"

#include "Viking/Events/ApplicationEvent.h"
#include "Viking/Events/MouseEvent.h"
#include "Viking/Events/KeyEvent.h"

#include "Viking/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Viking {
    float Window::sHighDPIScaleFactor{ 1.0f };
    static uint8_t sGLFWWindowCount{0};

    static void GLFWErrorCallback(int error, const char* description)
    {
        VI_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    WindowsWindow::WindowsWindow(const WindowProps props) {
        VI_PROFILE_FUNCTION();

        init(props);
    }

    WindowsWindow::~WindowsWindow() {
        VI_PROFILE_FUNCTION();

        shutdown();
    }

    void WindowsWindow::onUpdate() {
        VI_PROFILE_FUNCTION();

        glfwPollEvents();
        mContext->swapBuffers();
    }

    uint32_t WindowsWindow::getWidth() const {
        return mData.Width;
    }

    uint32_t WindowsWindow::getHeight() const {
        return mData.Height;
    }

    void WindowsWindow::setEventCallback(const EventCallbackFn &callback) {
        mData.EventCallback = callback;
    }

    void WindowsWindow::setVSync(bool enabled) {
        VI_PROFILE_FUNCTION();

        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        mData.VSync = enabled;
    }

    bool WindowsWindow::isVSync() const {
        return mData.VSync;
    }

    void *WindowsWindow::getNativeWindow() const {
        return mWindow;
    }

    void WindowsWindow::init(const WindowProps &props) {
        VI_PROFILE_FUNCTION();

        mData.Title = props.Title;
        mData.Width = props.Width;
        mData.Height = props.Height;

        VI_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (sGLFWWindowCount == 0)
        {
            VI_PROFILE_SCOPE("glfwInit");
            int success = glfwInit();
            VI_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        {
            VI_PROFILE_SCOPE("glfwCreateWindow");
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            float xScale, yScale;
            glfwGetMonitorContentScale(monitor, &xScale, &yScale);
            if (xScale > 1.0f || yScale > 1.0f)
            {
                sHighDPIScaleFactor = yScale;
                glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
            }

#if defined(VI_DEBUG)
            if (Renderer::getAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
            mWindow = glfwCreateWindow((int)props.Width, (int)props.Height, mData.Title.c_str(), nullptr, nullptr);
            ++sGLFWWindowCount;
        }

        mContext = GraphicsContext::create(mWindow);
        mContext->init();

        glfwSetWindowUserPointer(mWindow, &mData);
        setVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(mWindow, [](GLFWwindow* window, unsigned int keycode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event(keycode);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            data.EventCallback(event);
        });
    }

    void WindowsWindow::shutdown() {
        VI_PROFILE_FUNCTION();

        glfwDestroyWindow(mWindow);
        --sGLFWWindowCount;

        if (sGLFWWindowCount == 0)
        {
            glfwTerminate();
        }
    }
}
