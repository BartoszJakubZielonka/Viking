//
// Created by batzi on 15.05.2021.
//
#include "vipch.h"
#include "Application.h"

#include "Viking/Core/Log.h"

#include "Viking/Renderer/Renderer.h"

#include "Viking/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Viking {
    Application* Application::sInstance{nullptr};

    Application::Application(const std::string &name, ApplicationCommandLineArgs args): mCommandLineArgs(args) {
        VI_PROFILE_FUNCTION();

        VI_CORE_ASSERT(!sInstance, "Application already exists");
        sInstance = this;
        mWindow = Window::create(WindowProps(name));
        mWindow->setEventCallback(VI_BIND_EVENT_FN(Application::onEvent));

        Renderer::init();

        mImGuiLayer = new ImGuiLayer();
        pushOverlay(mImGuiLayer);
    }

    Application::~Application() {
        VI_PROFILE_FUNCTION();

        Renderer::shutdown();
    }

    void Application::pushLayer(Layer *layer) {
        VI_PROFILE_FUNCTION();

        mLayerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Layer *layer) {
        VI_PROFILE_FUNCTION();

        mLayerStack.pushOverlay(layer);
        layer->onAttach();
    }

    void Application::close() {
        mRunning = false;
    }

    void Application::onEvent(Event &e) {
        VI_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(VI_BIND_EVENT_FN(Application::onWindowClose));
        dispatcher.dispatch<WindowResizeEvent>(VI_BIND_EVENT_FN(Application::onWindowResize));

        for (auto it = mLayerStack.rbegin(); it != mLayerStack.rend(); ++it) {
            if (e.handled)
                break;
            (*it)->onEvent(e);
        }
    }

    void Application::run() {
        VI_PROFILE_FUNCTION();

        while(mRunning) {
            VI_PROFILE_SCOPE("RunLoop");

            auto time = (float)glfwGetTime();
            Timestep timestep = time - mLastFrameTime;
            mLastFrameTime = time;

            if(!mMinimized) {
                {
                    VI_PROFILE_SCOPE("LayerStack OnUpdate");
                    for (Layer* layer : mLayerStack)
                        layer->onUpdate(timestep);
                }

                mImGuiLayer->begin();
                {
                    VI_PROFILE_SCOPE("LayerStack OnImGuiRender");
                    for (Layer* layer : mLayerStack)
                        layer->onImGuiRender();
                }
                mImGuiLayer->end();
            }

            mWindow->onUpdate();
        }
    }

    bool Application::onWindowClose(WindowCloseEvent& e) {
        mRunning = false;
        return true;
    }

    bool Application::onWindowResize(WindowResizeEvent& e) {
        VI_PROFILE_FUNCTION();

        if (e.getWidth() == 0 || e.getHeight() == 0)
        {
            mMinimized = true;
            return false;
        }

        mMinimized = false;
        Renderer::onWindowResize(e.getWidth(), e.getHeight());

        return false;
    }
}