//
// Created by batzi on 15.05.2021.
//

#ifndef VIKING_APPLICATION_H
#define VIKING_APPLICATION_H

#include "Viking/Core/Base.h"

#include "Viking/Core/Window.h"
#include "Viking/Core/LayerStack.h"
#include "Viking/Events/Event.h"
#include "Viking/Events/ApplicationEvent.h"

#include "Viking/Core/Timestep.h"

#include "Viking/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Viking {
    struct ApplicationCommandLineArgs {
        int Count{0};
        char** Args{nullptr};

        const char* operator[](int index) const {
            VI_CORE_ASSERT(index < Count);
            return Args[index];
        }
    };

    class Application {
    public:
        Application(const std::string& name = "Viking App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
        virtual ~Application();

        void onEvent(Event& e);

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* layer);

        Window& getWindow() {
            return *mWindow;
        }

        void close();

        ImGuiLayer* getImGuiLayer() {
            return mImGuiLayer;
        }

        static Application& get() {
            return *sInstance;
        }

        ApplicationCommandLineArgs getCommandLineArgs() const {
            return mCommandLineArgs;
        }

    private:
        void run();

        bool onWindowClose(WindowCloseEvent& e);
        bool onWindowResize(WindowResizeEvent& e);

        ApplicationCommandLineArgs mCommandLineArgs;
        Scope<Window> mWindow;
        ImGuiLayer* mImGuiLayer;
        bool mRunning{true};
        bool mMinimized{false};
        LayerStack mLayerStack;
        float mLastFrameTime{0.0f};

        static Application* sInstance;
        friend int ::main(int argc, char** argv);
    };

    //To be defined in CLIENT APP
    Application* createApplication(ApplicationCommandLineArgs args);
}

#endif //VIKING_APPLICATION_H
